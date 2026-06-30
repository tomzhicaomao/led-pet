## ADDED Requirements

### Requirement: Three-button GPIO input
The system SHALL read three physical buttons connected to GPIO pins, with debouncing to reject false triggers.

#### Scenario: Button press detected
- **WHEN** a button is pressed and held for at least 50ms
- **THEN** the system registers a valid press event for that button

#### Scenario: Bounce rejection
- **WHEN** a button contact bounces within a 50ms window
- **THEN** only one press event is registered, not multiple

### Requirement: Button role mapping
The system SHALL map the three buttons to consistent roles: A (feed/confirm), B (interact/navigate), C (cancel/back).

#### Scenario: Feed with A button
- **WHEN** button A is pressed during normal idle
- **THEN** the feed action is triggered

#### Scenario: Menu navigation with B button
- **WHEN** a menu or interaction prompt is active and button B is pressed
- **THEN** the selection advances to the next option

#### Scenario: Cancel with C button
- **WHEN** a menu or prompt is active and button C is pressed
- **THEN** the menu closes and returns to normal mode

### Requirement: Long-press detection
The system SHALL distinguish between short presses (< 1 second) and long presses (>= 1 second).

#### Scenario: Short press
- **WHEN** a button is pressed and released within 1 second
- **THEN** a SHORT_PRESS event is dispatched

#### Scenario: Long press
- **WHEN** a button is held for 1 second or more
- **THEN** a LONG_PRESS event is dispatched

### Requirement: ISR-based capture
The system SHALL use GPIO interrupt service routines to capture button press timestamps, decoupled from the main render loop.

#### Scenario: Button pressed during render
- **WHEN** a button is pressed while the main loop is busy with TFT SPI transfer
- **THEN** the press is captured by ISR and processed on the next main loop iteration without loss
