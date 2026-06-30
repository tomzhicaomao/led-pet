## ADDED Requirements

### Requirement: Per-pet NVS save slots
The system SHALL save and load game state independently for each pet species using NVS keys keyed by pet index.

#### Scenario: Save and load pet 0
- **WHEN** game state is saved for pet index 0 and the device reboots
- **THEN** loading pet index 0 restores the exact points, stage, feedCount, foodBag, and lastFeedTime

#### Scenario: Multiple pets maintain independent state
- **WHEN** pet 0 has points=50 and pet 1 has points=20, and both are saved
- **THEN** loading pet 0 returns points=50 and loading pet 1 returns points=20

### Requirement: Save on state change
The system SHALL persist game state to NVS only when meaningful state changes occur (feed, evolve, reward, purchase).

#### Scenario: Save triggers
- **WHEN** any of feed(), evolve(), addPoints(), or buyFood() succeeds
- **THEN** the updated state is immediately persisted to NVS

#### Scenario: No save on no-op
- **WHEN** no game state has changed
- **THEN** no NVS write occurs

### Requirement: Active pet tracking
The system SHALL persist which pet is currently active across reboots.

#### Scenario: Active pet survives reboot
- **WHEN** the user switches to pet index 2 and the device reboots
- **THEN** pet index 2 is loaded as the active pet

### Requirement: PIN persistence
The system SHALL persist the 4-digit parent PIN in NVS with a default of "0000".

#### Scenario: Default PIN
- **WHEN** a new device is first booted
- **THEN** the parent PIN is "0000"

#### Scenario: Custom PIN survives reboot
- **WHEN** the parent changes the PIN and the device reboots
- **THEN** the PIN remains as set

### Requirement: Data versioning
The system SHALL include a version number in the save data to allow future format migrations.

#### Scenario: Version tag
- **WHEN** game state is saved
- **THEN** the serialized data includes a version field (uint32_t) alongside the state

### Requirement: Factory reset
The system SHALL support resetting all game state to defaults via a button combination.

#### Scenario: Reset via long-press
- **WHEN** buttons A and C are held together for 5 seconds
- **THEN** all saved state is cleared and the pet resets to stage 0 with default values
