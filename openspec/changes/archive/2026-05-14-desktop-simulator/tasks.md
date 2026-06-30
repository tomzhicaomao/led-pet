## 1. Core simulator framework

- [x] 1.1 Create `simulator.py`: tkinter window with Canvas, title "LED Pet Simulator"
- [x] 1.2 Implement RGB565 sprite parser: read .h file, extract uint16_t array, convert to RGB tuples
- [x] 1.3 Implement frame buffer: 320×240 RGB array with fill, blit, draw_text operations
- [x] 1.4 Implement Canvas renderer: frame buffer → PIL Image → ImageTk → Canvas at 3x zoom
- [x] 1.5 Implement game loop with tkinter `after(33, tick)` for ~30fps

## 2. Game logic integration

- [x] 2.1 Import and wire MockESP32 from tests/helpers/mock_esp32.py
- [x] 2.2 Implement Python PetAI class: 6 primary states, 3 idle sub-states, same transitions as C++ pet_ai.h
- [x] 2.3 Wire keyboard events: 1=A/Feed, 2=B/Interact, 3=C/Back
- [x] 2.4 Wire R=Reward+5, F=QuickFeed
- [x] 2.5 Integrate hunger timer (simulated time via counter, not millis)

## 3. Rendering features

- [x] 3.1 Render status bar at top: name, stage, points, AI state
- [x] 3.2 Render pet sprite at current position with animation frame cycling
- [x] 3.3 Render celebration particles: random white dots during HAPPY state
- [x] 3.4 Implement pet movement: smooth interpolation toward target position
- [x] 3.5 Render button hints at bottom of window

## 4. Polish and testing

- [x] 4.1 Add screenshot save with S key
- [x] 4.2 Add Esc to exit
- [x] 4.3 Verify all existing pytest tests still pass (simulator doesn't break test module)
- [x] 4.4 Add inline speed control: 1-9 keys for simulation speed multiplier
