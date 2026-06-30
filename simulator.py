#!/usr/bin/env python3
"""LED Pet Simulator - TFT display emulator with tkinter.

Usage:
    python simulator.py               # normal speed
    python simulator.py --speed 5     # 5x speed

Keys:
    1/2/3    = Button A/B/C (Feed / Interact / Cancel)
    R        = Parent sends +5 reward
    F        = Quick feed (any food)
    B        = Buy food (apple)
    E        = Evolution stone feed
    S        = Save screenshot
    1-9      = Speed multiplier
    Esc      = Exit
"""

import tkinter as tk
import sys
import re
import random
import os
from pathlib import Path
from datetime import datetime

# Add project root to path for imports
PROJECT_ROOT = Path(__file__).parent
sys.path.insert(0, str(PROJECT_ROOT / "tests"))

try:
    from PIL import Image, ImageTk
    HAS_PIL = True
except ImportError:
    HAS_PIL = False

from helpers.mock_esp32 import MockESP32

# ─── RGB565 Parser ──────────────────────────────────────────────

def rgb565_to_rgb(val):
    """Convert 16-bit RGB565 to (R, G, B) tuple."""
    r = ((val >> 11) & 0x1F) << 3
    g = ((val >> 5) & 0x3F) << 2
    b = (val & 0x1F) << 3
    return (r, g, b)


def parse_rgb565_header(filepath, var_name):
    """Extract RGB565 array from a C header file. Returns (width, height, pixels list)."""
    text = Path(filepath).read_text()

    # Extract width/height defines
    w = h = 0
    pattern = rf'#define {var_name.upper()}_WIDTH\s+(\d+)'
    m = re.search(pattern, text)
    if m:
        w = int(m.group(1))

    pattern_height = rf'#define {var_name.upper()}_HEIGHT\s+(\d+)'
    m = re.search(pattern_height, text)
    if m:
        h = int(m.group(1))

    # Extract array values
    array_start = text.find(f'const uint16_t {var_name}')
    if array_start < 0:
        # Try alternate name
        array_start = text.find(f'const uint16_t {var_name}[]')
    if array_start < 0:
        raise ValueError(f"Sprite array '{var_name}' not found in {filepath}")

    # Find the opening brace after the =
    brace = text.find('{', array_start)
    if brace < 0:
        raise ValueError(f"No opening brace found for {var_name}")

    # Extract hex values
    content = text[brace:]
    values = re.findall(r'0x[0-9A-Fa-f]{4}', content)

    pixels = [rgb565_to_rgb(int(v, 16)) for v in values]

    # Auto-detect size from array length if defines missing
    if w == 0 or h == 0:
        total = len(pixels)
        if w > 0:
            h = total // w
        elif h > 0:
            w = total // h
        else:
            # Assume square
            import math
            side = int(math.sqrt(total))
            if side * side == total:
                w = h = side
            else:
                w, h = total, 1

    return w, h, pixels


# ─── Python PetAI ────────────────────────────────────────────────

HUNGER_MS = 28800000  # 8 hours in ms
SLEEP_START = 21
SLEEP_END = 7

class PythonPetAI:
    """Mirrors C++ PetAI state machine."""

    PSTATE_IDLE    = 0
    PSTATE_HUNGRY  = 1
    PSTATE_EATING  = 2
    PSTATE_HAPPY   = 3
    PSTATE_SLEEPING = 4
    PSTATE_EVOLVING = 5

    STATE_NAMES = ["IDLE", "HUNGRY", "EATING", "HAPPY", "SLEEPING", "EVOLVING"]

    ISUB_ROAMING   = 0
    ISUB_SITTING   = 1
    ISUB_CURIOSITY = 2

    def __init__(self):
        self.state = self.PSTATE_IDLE
        self.sub_state = self.ISUB_ROAMING
        self.frame_index = 0
        self.frame_timer = 0
        self.last_feed_time = 0
        self.state_enter_time = 0
        self.sub_state_timer = 0
        self.queued_reward = False
        self.anim = 0  # ANIM_IDLE

        self.x = 140
        self.y = 120
        self.target_x = 140
        self.target_y = 120
        self.moving = False

        self.sim_time = 0  # simulated millis

    def is_hungry(self):
        if self.last_feed_time == 0:
            return True
        return (self.sim_time - self.last_feed_time) >= HUNGER_MS

    def is_sleep_time(self, hour):
        return hour >= SLEEP_START or hour < SLEEP_END

    def transition(self, new_state):
        if self.state == new_state:
            return
        self.state = new_state
        self.state_enter_time = self.sim_time
        self.frame_index = 0

        anim_map = {
            self.PSTATE_IDLE: 0, self.PSTATE_HUNGRY: 0,
            self.PSTATE_EATING: 1, self.PSTATE_HAPPY: 2,
            self.PSTATE_SLEEPING: 3, self.PSTATE_EVOLVING: 2,
        }
        self.anim = anim_map.get(new_state, 0)

    def update(self, dt_ms, hour=12, ntp_valid=True):
        self.sim_time += dt_ms

        # Process queued reward
        if self.queued_reward and self.state != self.PSTATE_SLEEPING:
            self.queued_reward = False
            self.transition(self.PSTATE_HAPPY)

        if self.state == self.PSTATE_IDLE:
            if self.is_hungry():
                self.transition(self.PSTATE_HUNGRY)
            elif ntp_valid and self.is_sleep_time(hour):
                self.transition(self.PSTATE_SLEEPING)
            else:
                # Sub-state logic
                if self.sim_time - self.sub_state_timer >= 30000 + random.randint(0, 30000):
                    self._pick_sub_state()
                self._update_movement()

        elif self.state == self.PSTATE_HUNGRY:
            if ntp_valid and self.is_sleep_time(hour):
                self.transition(self.PSTATE_SLEEPING)
            elif not self.is_hungry():
                self.transition(self.PSTATE_IDLE)

        elif self.state == self.PSTATE_EATING:
            if self.sim_time - self.state_enter_time >= 2000:
                self.transition(self.PSTATE_HAPPY)

        elif self.state == self.PSTATE_HAPPY:
            if self.sim_time - self.state_enter_time >= 3000:
                self.transition(self.PSTATE_IDLE)

        elif self.state == self.PSTATE_SLEEPING:
            if ntp_valid and not self.is_sleep_time(hour):
                self.transition(self.PSTATE_IDLE)

        elif self.state == self.PSTATE_EVOLVING:
            if self.sim_time - self.state_enter_time >= 4000:
                self.transition(self.PSTATE_HAPPY)

        # Advance animation frame
        self.frame_timer += dt_ms
        if self.frame_timer >= 400:  # frame duration
            self.frame_timer = 0
            self.frame_index = (self.frame_index + 1) % 2  # 2 frames per anim

    def _pick_sub_state(self):
        self.sub_state_timer = self.sim_time
        r = random.randint(0, 100)
        if r < 60:
            self.sub_state = self.ISUB_ROAMING
            self.target_x = random.randint(40, 280)
            self.target_y = random.randint(60, 200)
            self.moving = True
        elif r < 90:
            self.sub_state = self.ISUB_SITTING
            self.moving = False
        else:
            self.sub_state = self.ISUB_CURIOSITY
            self.target_x = random.randint(40, 280)
            self.target_y = random.randint(60, 200)
            self.moving = True

    def _update_movement(self):
        if not self.moving:
            return
        dx = self.target_x - self.x
        dy = self.target_y - self.y
        if abs(dx) <= 1 and abs(dy) <= 1:
            self.x, self.y = self.target_x, self.target_y
            self.moving = False
            return
        if abs(dx) > 1:
            self.x += 1 if dx > 0 else -1
        if abs(dy) > 1:
            self.y += 1 if dy > 0 else -1

    def on_feed(self):
        self.last_feed_time = self.sim_time
        self.transition(self.PSTATE_EATING)

    def on_reward(self):
        if self.state == self.PSTATE_SLEEPING:
            self.queued_reward = True
        else:
            self.transition(self.PSTATE_HAPPY)

    def on_evolve(self):
        self.transition(self.PSTATE_EVOLVING)


# ─── Frame Buffer ────────────────────────────────────────────────

TFT_W, TFT_H = 320, 240

class FrameBuffer:
    def __init__(self):
        self.buf = [(0, 0, 0)] * (TFT_W * TFT_H)

    def fill(self, color):
        self.buf = [color] * (TFT_W * TFT_H)

    def set_pixel(self, x, y, color):
        if 0 <= x < TFT_W and 0 <= y < TFT_H:
            self.buf[y * TFT_W + x] = color

    def blit_sprite(self, sprite_data, spr_w, spr_h, dst_x, dst_y):
        """Blit sprite data (list of RGB tuples) at dst_x, dst_y."""
        pixels = sprite_data
        for row in range(spr_h):
            for col in range(spr_w):
                color = pixels[row * spr_w + col]
                if color == (0, 0, 0):  # transparent
                    continue
                px = dst_x + col
                py = dst_y + row
                if 0 <= px < TFT_W and 0 <= py < TFT_H:
                    self.buf[py * TFT_W + px] = color

    def fill_rect(self, x, y, w, h, color):
        for row in range(y, y + h):
            for col in range(x, x + w):
                self.set_pixel(col, row, color)

    def draw_text_char(self, ch, x, y, color):
        """Draw a single 5x7 pixel character (digits 0-9 only)."""
        digits = [
            [0x7C, 0x82, 0x82, 0x82, 0x7C],  # 0
            [0x00, 0x42, 0xFE, 0x02, 0x00],  # 1
            [0x46, 0x8A, 0x92, 0xA2, 0x42],  # 2
            [0x44, 0x82, 0x92, 0x92, 0x6C],  # 3
            [0x30, 0x28, 0x24, 0xFE, 0x20],  # 4
            [0xF2, 0x92, 0x92, 0x92, 0x8C],  # 5
            [0x7C, 0x92, 0x92, 0x92, 0x4C],  # 6
            [0x80, 0x8E, 0x90, 0xA0, 0xC0],  # 7
            [0x6C, 0x92, 0x92, 0x92, 0x6C],  # 8
            [0x64, 0x92, 0x92, 0x92, 0x7C],  # 9
        ]
        if '0' <= ch <= '9':
            glyph = digits[ord(ch) - ord('0')]
            for col_idx in range(5):
                bits = glyph[col_idx]
                for row_idx in range(7):
                    if bits & (1 << row_idx):
                        self.set_pixel(x + col_idx, y + row_idx, color)

    def draw_text(self, text, x, y, color=(255, 255, 255)):
        """Draw text using built-in pixel font (digits + simple uppercase)."""
        cx = x
        for ch in text:
            if '0' <= ch <= '9':
                self.draw_text_char(ch, cx, y, color)
                cx += 6
            elif ch == ' ':
                cx += 4
            else:
                # Simple abbreviation: skip non-digit chars
                # Draw as a dot marker for readability
                self.set_pixel(cx + 2, y + 3, color)
                cx += 6


# ─── Main Simulator ──────────────────────────────────────────────

class PetSimulator:
    def __init__(self, speed=1):
        self.speed = speed
        self.root = tk.Tk()
        self.root.title("LED Pet Simulator")
        self.root.resizable(False, False)

        # Canvas: 3x zoom
        self.zoom = 3
        self.canvas = tk.Canvas(
            self.root,
            width=TFT_W * self.zoom,
            height=TFT_H * self.zoom + 40,
            bg="#1a1a2e"
        )
        self.canvas.pack()

        # Status label
        self.status_label = tk.Label(
            self.root,
            text="Loading...",
            font=("Menlo", 14),
            bg="#1a1a2e", fg="#ffffff"
        )
        self.status_label.pack(pady=4)

        # Load sprite
        sprite_path = PROJECT_ROOT / "src" / "pet" / "sprite_test.h"
        self.sprite_w, self.sprite_h, self.sprite_pixels = parse_rgb565_header(
            str(sprite_path), "test_sprite_dragon"
        )
        print(f"Loaded sprite: {self.sprite_w}x{self.sprite_h}")

        # Init game and AI
        self.game = MockESP32()
        self.ai = PythonPetAI()

        # State
        self.frame_buf = FrameBuffer()
        self.hour = 12
        self.ntp_valid = True
        self.running = True
        self.screenshot_count = 0

        # Pet metadata
        self.pet_name = "Dragon"
        self.stage_names = ["Egg", "Baby", "Teen", "Adult", "Final"]

        # Particle system
        self.particles = []  # [(x, y, life)]

        # Bind keys
        self.root.bind("<Key>", self.on_key)
        self.root.bind("<Escape>", lambda e: self.quit())
        self.root.protocol("WM_DELETE_WINDOW", self.quit)

        self.canvas.focus_set()

        # Bottom hint
        self.hint_label = tk.Label(
            self.root,
            text="1=Feed  2=Interact  3=Cancel  R=Reward+5  F=QuickFeed  B=Buy  E=Evolve  S=Screenshot  1-9=Speed  Esc=Exit",
            font=("Menlo", 9),
            bg="#1a1a2e", fg="#666688"
        )
        self.hint_label.pack(pady=2)

        # Start
        self.root.after(33, self.tick)
        self.root.mainloop()

    def on_key(self, event):
        ch = event.char.upper() if event.char else event.keysym.upper()
        if ch == '1':
            # Feed button A
            r = self.game.feed(0)
            if r["ok"]:
                self.ai.on_feed()
                if r["evolved"]:
                    self.ai.on_evolve()
            else:
                print("Feed failed: no food")

        elif ch == '2':
            # Interact button B - buy apple
            if self.game.buy(0):
                print("Bought apple")

        elif ch == '3':
            # Cancel button C
            print("Cancel / long press = factory reset simulation")

        elif ch == 'R':
            # Reward +5
            ok = self.game.send_reward(5, "0000", "keyboard")
            if ok:
                self.ai.on_reward()
                print("Reward +5!")

        elif ch == 'F':
            # Quick feed - ensure food, then feed
            self.game.game.state["foodBag"][0] = max(self.game.game.state["foodBag"][0], 1)
            r = self.game.feed(0)
            if r["ok"]:
                self.ai.on_feed()
                if r["evolved"]:
                    self.ai.on_evolve()
                print("Quick feed!")

        elif ch == 'B':
            # Buy apple
            if self.game.buy(0):
                print("Bought apple")

        elif ch == 'E':
            # Evolution stone
            self.game.game.state["foodBag"][4] = max(self.game.game.state["foodBag"][4], 1)
            r = self.game.feed(4)
            if r["ok"]:
                self.ai.on_evolve()
                print("EVOLVING!")
            else:
                print("Evolution failed (need more feedCount)")

        elif ch == 'S':
            self.save_screenshot()

        elif ch in '123456789':
            self.speed = int(ch)
            print(f"Speed: {self.speed}x")

    def save_screenshot(self):
        """Save current frame buffer as PNG."""
        try:
            from PIL import Image
        except ImportError:
            print("Pillow not installed. pip install Pillow")
            return

        img = Image.new("RGB", (TFT_W, TFT_H))
        img.putdata(self.frame_buf.buf)
        name = f"screenshot_{datetime.now().strftime('%Y%m%d_%H%M%S')}.png"
        img.save(name)
        self.screenshot_count += 1
        print(f"Screenshot saved: {name}")

    def tick(self):
        if not self.running:
            return

        dt_ms = int(33 * self.speed)

        # Update AI
        self.ai.update(dt_ms, self.hour, self.ntp_valid)

        # Update particles
        new_particles = []
        for px, py, life in self.particles:
            life -= dt_ms / 1000.0
            if life > 0:
                new_particles.append((px + random.randint(-1, 1), py - 1, life))
        self.particles = new_particles

        # Add particles during HAPPY state
        if self.ai.state == PythonPetAI.PSTATE_HAPPY:
            for _ in range(5):
                self.particles.append((
                    random.randint(0, TFT_W - 1),
                    random.randint(0, TFT_H - 1),
                    1.0 + random.random()
                ))

        # ─── Render ───
        self.frame_buf.fill((0x4D >> 1, 0x2A >> 1, 0x2A >> 1))  # dark green bg

        # Draw ground
        self.frame_buf.fill_rect(0, TFT_H - 20, TFT_W, 20, (34, 51, 17))

        # Draw pet sprite
        anim_frame = self.ai.frame_index
        spr_start = anim_frame * self.sprite_w * self.sprite_h
        spr_pixels = self.sprite_pixels[spr_start:spr_start + self.sprite_w * self.sprite_h]

        # If sleeping, dim the sprite
        if self.ai.state == PythonPetAI.PSTATE_SLEEPING:
            spr_pixels = [
                (r // 3, g // 3, b // 3) if p != (0, 0, 0) else p
                for p in spr_pixels
            ]

        self.frame_buf.blit_sprite(spr_pixels, self.sprite_w, self.sprite_h, self.ai.x, self.ai.y)

        # Draw particles
        for px, py, _ in self.particles:
            if 0 <= px < TFT_W and 0 <= py < TFT_H:
                self.frame_buf.set_pixel(px, py, (255, 255, 0))

        # Draw status bar background
        self.frame_buf.fill_rect(0, 0, TFT_W, 18, (4, 2, 16))  # dark

        # Draw status text
        gs = self.game.game.state
        status = f"{self.pet_name} {PythonPetAI.STATE_NAMES[self.ai.state]} STG:{gs['stage']} PTS:{gs['points']}"
        self.frame_buf.draw_text(status, 4, 2, (255, 255, 255))

        # Update status label
        self.status_label.config(
            text=f"⭐{gs['points']}pts | {self.pet_name} | "
                 f"Stage:{self.stage_names[gs['stage']]} | "
                 f"{PythonPetAI.STATE_NAMES[self.ai.state]} | "
                 f"Food:{sum(gs['foodBag'])} | "
                 f"FeedCnt:{gs['feedCount']} | "
                 f"Speed:{self.speed}x"
        )

        # Render to canvas
        try:
            if HAS_PIL:
                img = Image.new("RGB", (TFT_W, TFT_H))
                img.putdata(self.frame_buf.buf)
                img = img.resize((TFT_W * self.zoom, TFT_H * self.zoom), Image.NEAREST)
                self.photo = ImageTk.PhotoImage(img)
                self.canvas.create_image(0, 0, anchor=tk.NW, image=self.photo)
            else:
                # Fallback: pixel-by-pixel (slow but zero dependencies)
                self.canvas.delete("all")
                for row in range(0, TFT_H, 2):  # skip every other row for speed
                    for col in range(0, TFT_W, 2):
                        c = self.frame_buf.buf[row * TFT_W + col]
                        hex_color = f'#{c[0]:02x}{c[1]:02x}{c[2]:02x}'
                        self.canvas.create_rectangle(
                            col * self.zoom, row * self.zoom,
                            (col + 2) * self.zoom, (row + 2) * self.zoom,
                            fill=hex_color, outline=""
                        )
        except Exception as e:
            print(f"Render error: {e}")

        # Schedule next tick
        self.root.after(max(16, 33 // self.speed), self.tick)

    def quit(self):
        self.running = False
        self.root.destroy()


if __name__ == '__main__':
    speed = 1
    if len(sys.argv) > 2 and sys.argv[1] == '--speed':
        speed = int(sys.argv[2])
    print(f"LED Pet Simulator (speed: {speed}x)")
    print("Keys: 1=Feed 2=Buy 3=Cancel R=Reward F=ForceFeed E=Evolve S=Screenshot Esc=Exit")
    PetSimulator(speed=speed)
