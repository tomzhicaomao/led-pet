## Why

The current 32x8 monochrome LED matrix cannot render recognizable pet characters with sufficient detail -- 8 rows leave no room for facial features, expressions, or visual differentiation between evolution stages. Swapping to a 2.4" TFT (320x240, 65K colors) costs under 20 RMB and unlocks Minecraft-style pixel art that an 8-year-old can emotionally connect with. The product goal is shifting from "LED demo board" to "physical habit-building companion device" -- a standalone desk toy for the child plus a parent-only web management page for reward distribution.

## What Changes

- **BREAKING**: Replace MD_MAX72XX LED matrix driver with TFT_eSPI (ILI9341) display driver
- **BREAKING**: Replace 32x8 monochrome frame buffer with 320x240 full-color sprite renderer
- Replace demo-mode animation cycling with autonomous pet AI state machine (idle / hungry / eating / happy / sleeping / evolve)
- Add 3-button GPIO input subsystem with debouncing
- Add multi-species pet registry with per-species pixel art sprites
- Add evolution system with visually distinct stages per species
- Add parent reward API: parent sends points via web page, device plays celebration animation
- Add NTP time synchronization for day/night pet behavior rhythm
- Add per-pet independent NVS save slots
- Add buzzer subsystem for audio feedback
- **BREAKING**: Replace SPIFFS-hosted interactive web UI with a parent-only management page (child does not use a phone)
- Migrate existing game logic (points, feeding, evolution thresholds, food shop) from LED matrix codebase, adapted to TFT rendering and button input

## Capabilities

### New Capabilities

- `tft-display`: TFT_eSPI display initialization, sprite rendering engine, double-buffered draw pipeline for 320x240 ILI9341
- `pixel-pet-rendering`: Minecraft-style pixel art pet sprites (16x16 to 32x32), multi-species registry, stage-based sprite switching, celebration and emotion overlays
- `pet-ai-state-machine`: Autonomous pet behavior -- idle roaming, hunger detection, eating response, happiness bursts, sleep at night, evolution events. Time-driven with NTP sync.
- `button-input`: 3-button GPIO input with debouncing, mapped to feed/confirm, interact/navigate, cancel/back
- `game-systems`: Points wallet, food shop with 5 item types, 5-stage evolution with configurable feed thresholds, food inventory with stack limits
- `persistence`: Multi-slot NVS save/load, per-pet independent progress, PIN-protected parent settings
- `parent-web-console`: Minimal parent-only web page served from SPIFFS -- reward point distribution with reason tags, PIN auth, pet status overview
- `buzzer-feedback`: PWM-driven passive buzzer for celebration jingles and interaction sounds

### Modified Capabilities

None -- greenfield architecture replacing the LED matrix codebase. Some game logic algorithms (feeding math, evolution thresholds) will be adapted from existing `src/game/` but the interfaces and data flow are entirely new.

## Impact

- **Display**: `src/display/` replaced entirely -- new `tft_driver.h/cpp`, `sprite_renderer.h/cpp`, `pixel_font.h`
- **Pet**: `src/pet/` restructured -- `species/` subdirectory per pet, new `pet_ai.h/cpp` state machine, existing `pet_data.h` adapted for color sprites
- **Game**: `src/game/` migrated -- `game_state.h/cpp`, `persistence.h/cpp` adapted from existing code; new `evolution.h/cpp`
- **Input**: New `src/input/` module -- `button_manager.h/cpp`
- **Audio**: New `src/audio/` module -- `buzzer.h/cpp`
- **Web**: `src/web/` adapted -- `web_server.h/cpp` reduced to parent-only API + management page; existing `wifi_manager.h/cpp` retained
- **Build**: `platformio.ini` -- replace MD_MAX72XX dep with TFT_eSPI, add NTP client lib
- **Tests**: `tests/` expanded with comprehensive API-level game logic tests via MockESP32 fixture
