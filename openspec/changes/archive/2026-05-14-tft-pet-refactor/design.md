## Context

The existing led-pet codebase drives a 32x8 monochrome LED matrix (MD_MAX72XX) with a single hardcoded pet species and demo-mode animation cycling. The hardware is being upgraded to a 2.4" TFT ILI9341 (320x240, SPI, 65K colors). The product is being re-scoped from a web-controlled demo board to a standalone physical companion device for an 8-year-old child, with parent reward management via a separate web page.

The ESP32 Dev Board, PlatformIO build system, and Arduino framework are retained. The existing game logic (points, feeding, evolution) has working algorithms that can be adapted, but the display, input, and web layers are entirely replaced.

## Goals / Non-Goals

**Goals:**
- Pixel-art pet rendered on TFT with smooth sprite-based animation
- Autonomous pet behavior driven by a time-aware state machine
- 3-button physical input for the child
- Parent reward distribution via web page (parent only, child does not use phone)
- Multi-species pet system with per-species pixel art
- Multi-stage evolution with visually distinct sprites
- Persistent game state across power cycles (per-pet NVS slots)
- NTP time sync for day/night pet behavior
- Buzzer audio feedback for key events

**Non-Goals:**
- Touch screen interaction (physical buttons only)
- Cloud/server backend (ESP32 is self-contained on LAN)
- OTA firmware updates (out of scope for initial release)
- Wi-Fi configuration UI (use existing WiFiManager or hardcode credentials)
- Child-facing web or mobile app
- Multiplayer / device-to-device interaction
- Real-time clock hardware (use NTP over Wi-Fi)

## Decisions

### D1: TFT_eSPI over LovyanGFX

TFT_eSPI is the de-facto standard for ILI9341 on ESP32, has the largest community, and integrates cleanly with PlatformIO. LovyanGFX offers more features (sprite rotation, scaling) but adds complexity we don't need -- all sprites are pre-rendered at target size. TFT_eSPI's `pushImage()` with RGB565 is sufficient.

### D2: Hand-written sprite renderer over LVGL

LVGL is a GUI framework optimized for forms, buttons, and menus. Our UI is a game-like scene with moving sprites on a background -- a poor fit for LVGL's widget tree. A custom renderer is 200-300 lines of C++:

```
Render pipeline (per frame):
  1. Clear back buffer to sky color
  2. Blit static scene objects (trees, house, ground)
  3. Blit pet sprite at current (x, y)
  4. Blit UI overlay (status bar: points, mood icon)
  5. If celebration active: blit particle sprites
  6. Push back buffer to TFT
```

Sprite format: 16-bit RGB565 packed arrays in PROGMEM. Generated from PNG source files via a Python script (`scripts/png_to_rgb565.py`).

### D3: Arduino framework over ESP-IDF

The existing codebase is Arduino, TFT_eSPI and WiFiManager are Arduino libraries, and the project has no RTOS requirements. ESP-IDF would give finer control but adds complexity with no benefit for this scope.

### D4: Single-threaded event loop over FreeRTOS tasks

ESP32 has dual cores, but all our subsystems (display, input, game, web) operate at human timescales (milliseconds to seconds). A single-threaded `loop()` with cooperative scheduling avoids concurrency bugs:

```cpp
void loop() {
    uint32_t now = millis();
    buttons.update(now);       // ~10us (GPIO read)
    petAI.update(now);         // ~100us (state check)
    game.update(now);          // ~50us  (auto-save check)
    renderer.draw(now);        // ~30ms  (TFT SPI transfer)
    webServer.handleClient();  // ~1ms   (non-blocking)
    audio.update(now);         // ~100us (tone check)
    delay(16);                 // target ~30fps
}
```

### D5: NVS for game state, SPIFFS for web assets

Preferences/NVS is ideal for small structured data (`GameState` is ~32 bytes). SPIFFS serves the parent web page (HTML/CSS/JS, ~10KB total). No SD card needed.

### D6: Pet AI as a hierarchical state machine

```
Primary states: IDLE, HUNGRY, EATING, HAPPY, SLEEPING, EVOLVING

Transitions:
  IDLE -> HUNGRY:    hungerTimer expired (8 hours since last feed)
  IDLE/HUNGRY -> EATING: feed button pressed + food available
  EATING -> HAPPY:   eating animation complete
  IDLE -> SLEEPING:  NTP hour in sleep range (21:00-07:00)
  SLEEPING -> IDLE:  NTP hour in wake range (07:00-21:00)
  HAPPY -> IDLE:     happiness animation complete
  IDLE -> EVOLVING:  feedCount >= threshold AND evolution stone used
  EVOLVING -> IDLE:  evolution animation complete
  Any -> HAPPY:      parent reward received (preempts current state)

Sub-states (within IDLE):
  ROAMING:    pet walks around the scene
  SITTING:    pet rests in place briefly
  CURIOSITY:  pet investigates a random object
```

### D7: Sprite data architecture

Each pet species is a directory under `src/pet/species/` containing:
- `pet_<name>.h` -- extern declaration of the `PetSpecies` instance
- `pet_<name>.cpp` -- all sprite frame data (RGB565 arrays) + species definition

A `PetRegistry` singleton provides index-based lookup. Adding a new pet = new species file + one line in registry. Sprite arrays are `static const uint16_t[] PROGMEM` for zero RAM cost at rest.

### D8: Parent web console architecture

A single HTML page served from SPIFFS with three functions:
1. Reward point distribution (preset buttons + custom amount + reason tag)
2. Pet status overview (stage, mood, points)
3. PIN change

ESP32 serves it at `http://<device-ip>/` with CORS headers for dev proxy. API endpoints:

| Method | Path | Purpose |
|--------|------|---------|
| GET | /api/status | Pet state, points, stage |
| POST | /api/reward | Add points with reason and PIN |
| POST | /api/pin | Change PIN |
| GET | /api/history | Recent reward log (in-memory ring buffer) |

## Risks / Trade-offs

- **TFT SPI speed caps framerate**: ILI9341 at 40MHz SPI can push ~15-20fps full-screen. Acceptable for a pet toy. Mitigation: sprite-diff dirty-rect optimization if needed.
- **No RTC means offline time drift**: Without NTP, millis() resets on reboot. Pet loses day/night rhythm when offline. Mitigation: acceptable -- Wi-Fi is assumed for parent console anyway.
- **NVS write endurance**: ESP32 NVS rated ~100K write cycles. Auto-save every 60s would exhaust in ~70 days. Mitigation: save only on state changes (feed, evolve, reward), not on a timer.
- **Single-threaded loop blocks during SPI transfer**: TFT pushImage() takes ~30ms, during which button presses could be missed. Mitigation: button GPIO ISR captures press timestamps into a ring buffer; main loop drains it between frames.
- **Parent page accessible to child if they know the IP**: Management page has PIN protection for sensitive actions. Status view is read-only and safe.

## Open Questions

- Exact button GPIO pin assignment -- depends on final PCB/wiring layout
- Buzzer pin and whether to include in first build
- Specific pet species pixel art -- design work needed, not coding
- 3D-printed enclosure dimensions -- depends on final component layout
