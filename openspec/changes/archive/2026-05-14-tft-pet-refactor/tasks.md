## 1. Project scaffold and TFT display

- [x] 1.1 Update platformio.ini: remove MD_MAX72XX, add TFT_eSPI and NTPClient dependencies
- [x] 1.2 Create TFT_eSPI User_Setup.h with correct ILI9341 pin mappings
- [x] 1.3 Implement src/display/tft_driver.h/cpp: display init, fillScreen, brightness control
- [x] 1.4 Implement src/display/sprite_renderer.h/cpp: pushImage wrapper, layered scene render pipeline
- [x] 1.5 Create scripts/png_to_rgb565.py: convert PNG to const uint16_t PROGMEM arrays
- [x] 1.6 Create a test sprite (32x32 placeholder) and verify it renders on the TFT
- [x] 1.7 Implement src/display/pixel_font.h: minimal 5x7 pixel font for status bar text

## 2. Pet data structure and species registry

- [x] 2.1 Rename/adapt src/pet/pet_data.h: add RGB565 sprite pointers, color palette, species metadata
- [x] 2.2 Create src/pet/species/ directory and pet_dragon.h/cpp with placeholder 32x32 sprites
- [x] 2.3 Implement src/pet/pet_registry.h/cpp: PetRegistry singleton with index-based lookup and count
- [x] 2.4 Create src/pet/pet_sprite.h: sprite frame, animation clip (array of frames + timing + loop), animation set (idle/eating/happy/sleeping)
- [x] 2.5 Wire pet registry into main.cpp: load active pet on boot, render its current stage sprite to screen

## 3. Pet animation and AI state machine

- [x] 3.1 Implement src/pet/pet_ai.h/cpp: PetAI class with primary state enum and transition logic
- [x] 3.2 Implement sub-state machine: ROAMING, SITTING, CURIOSITY within IDLE state
- [x] 3.3 Implement target-based movement: pet smoothly moves toward random (x, y) targets within scene bounds
- [x] 3.4 Wire NTP time sync in setup(): fetch time after Wi-Fi connect, update pet sleep/wake schedule
- [x] 3.5 Implement sleep/wake behavior: pet transitions to SLEEPING 21:00-07:00, wakes with animation
- [x] 3.6 Implement hunger timer: track millis() since last feed, transition to HUNGRY after 8 hours
- [x] 3.7 Implement reward interrupt: preempt current state with HAPPY celebration, resume afterward
- [x] 3.8 Implement celebration particle effects: star/sparkle sprites animate for 2-3 seconds
- [x] 3.9 Integrate pet AI into main.cpp loop: AI update + renderer driven by current state

## 4. Game systems

- [x] 4.1 Implement src/game/game_state.h/cpp: GameState struct (points, stage, feedCount, foodBag, lastFeedTime, parentPIN, currentPet)
- [x] 4.2 Implement src/game/food_shop.h/cpp: 5 food items with prices/feed values/evolution stone flag
- [x] 4.3 Implement feeding logic: decrement foodBag, increment feedCount, update lastFeedTime
- [x] 4.4 Implement evolution logic: check feedCount >= stage threshold, increment stage, reset feedCount
- [x] 4.5 Implement points wallet: addPoints with PIN check, deduct for purchases
- [x] 4.6 Implement buyFood: validate points, check stack limit (99 max), add to inventory
- [x] 4.7 Wire game systems into pet AI: feeding triggers EATING state, evolution triggers EVOLVING state

## 5. Persistence

- [x] 5.1 Implement src/game/persistence.h/cpp: save( petIndex, GameState ) and load( petIndex, GameState )
- [x] 5.2 Implement save-on-change: only write NVS after feed, evolve, reward, or purchase success
- [x] 5.3 Implement active pet tracking: persist currentPet index, restore on boot
- [x] 5.4 Implement PIN persistence and verification
- [x] 5.5 Add data version field for future migration compatibility
- [x] 5.6 Implement factory reset: hold buttons A+C for 5 seconds clears all NVS data

## 6. Button input

- [x] 6.1 Implement src/input/button_manager.h/cpp: GPIO ISR captures timestamps into ring buffer
- [x] 6.2 Implement debouncing: 50ms dead time after first edge
- [x] 6.3 Implement short-press vs long-press detection (threshold: 1 second)
- [x] 6.4 Map buttons: A=feed/confirm, B=interact/navigate, C=cancel/back
- [x] 6.5 Wire buttons into main loop: drain ring buffer, dispatch events to pet AI and game engine

## 7. Parent web console

- [x] 7.1 Adapt src/web/wifi_manager.h/cpp: retain existing WiFi connection logic
- [x] 7.2 Adapt src/web/web_server.h/cpp: reduce to parent-only API endpoints, add CORS headers
- [x] 7.3 Implement API: GET /api/status (pet name, stage, points, mood, foodBag summary)
- [x] 7.4 Implement API: POST /api/reward (points, reason tag, PIN auth)
- [x] 7.5 Implement API: POST /api/pin (old PIN, new PIN)
- [x] 7.6 Implement API: GET /api/history (last 20 reward events from in-memory ring buffer)
- [x] 7.7 Create data/index.html: parent management page with reward buttons, pin input, pet status, history log
- [x] 7.8 Create data/style.css: clean mobile-friendly styling for parent page
- [x] 7.9 Create data/app.js: fetch API, reward submission, auto-refresh every 5s, PIN change dialog
- [x] 7.10 Set up Vite dev proxy config for frontend development without SPIFFS uploads

## 8. Buzzer feedback

- [x] 8.1 Implement src/audio/buzzer.h/cpp: PWM tone generation, note definitions, melody sequencer
- [x] 8.2 Implement non-blocking playback: melody advances via timer checks in main loop
- [x] 8.3 Map events to sounds: feed=ascending chime, reward=celebratory jingle, evolve=dramatic melody, error=low buzz
- [x] 8.4 Add silence mode toggle via persistent setting

## 9. Testing

- [x] 9.1 Update tests/helpers/mock_esp32.py: MockESP32 to match new API surface
- [x] 9.2 Write tests/test_game_logic.py: feed consumption, evolution thresholds, stack limits, PIN protection, hunger state
- [x] 9.3 Write tests/test_persistence.py: save/load per pet, versioning, factory reset
- [x] 9.4 Write tests/test_pet_ai.py: state transitions, hunger timer, sleep schedule, reward interrupt
- [x] 9.5 Write tests/test_reward_api.py: PIN auth, reason tags, history ring buffer
- [x] 9.6 Run full test suite and verify >= 80% coverage of game logic paths
