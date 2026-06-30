## ADDED Requirements

### Requirement: Display initialization
The system SHALL initialize an ILI9341 TFT display via SPI at 320x240 resolution on boot.

#### Scenario: Successful display init
- **WHEN** the device boots and TFT hardware is connected
- **THEN** the display initializes with correct orientation and the screen fills with a default background color

#### Scenario: Display init failure
- **WHEN** the device boots and TFT hardware is not connected
- **THEN** the system logs an error to Serial and continues operating in headless mode without crashing

### Requirement: Sprite blitting
The system SHALL render a 16-bit RGB565 sprite array at a specified (x, y) coordinate on the display.

#### Scenario: Blit sprite fully on-screen
- **WHEN** a 32x32 sprite is blitted at position (100, 80)
- **THEN** the sprite pixels appear at the correct location without distortion

#### Scenario: Blit sprite partially off-screen
- **WHEN** a sprite is blitted at a position where it extends beyond the display boundary
- **THEN** only the visible portion is rendered without memory corruption

### Requirement: Scene rendering pipeline
The system SHALL execute a layered render pipeline each frame: background color, static scene objects, pet sprite, UI overlay.

#### Scenario: Full frame render
- **WHEN** the render pipeline executes
- **THEN** the background fills first, scene objects render on top, the pet sprite renders above scene objects, and the status bar renders on the topmost layer

### Requirement: Status bar
The system SHALL render a status bar at the top of the screen showing current points and pet mood.

#### Scenario: Status bar update
- **WHEN** the game state changes (points added, mood changes)
- **THEN** the status bar reflects the updated values on the next frame

### Requirement: Celebration particle effects
The system SHALL render temporary particle sprites (stars, sparkles) during celebration states.

#### Scenario: Reward celebration
- **WHEN** a parent reward is received
- **THEN** particle sprites animate across the screen for 2-3 seconds before returning to normal rendering
