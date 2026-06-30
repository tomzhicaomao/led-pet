## ADDED Requirements

### Requirement: PWM tone generation
The system SHALL drive a passive buzzer via PWM to produce simple tones and melodies.

#### Scenario: Single tone
- **WHEN** a tone of 440Hz is requested for 200ms
- **THEN** the buzzer outputs a 440Hz square wave for exactly 200ms

#### Scenario: Tone sequence
- **WHEN** a melody of 3 notes is requested
- **THEN** the buzzer plays each note in sequence without overlap

### Requirement: Event-to-sound mapping
The system SHALL play distinct sound patterns for different game events.

#### Scenario: Feed sound
- **WHEN** the pet eats food
- **THEN** a short ascending two-note chime plays

#### Scenario: Reward celebration sound
- **WHEN** a parent reward is received
- **THEN** a longer celebratory jingle plays

#### Scenario: Evolution sound
- **WHEN** the pet evolves
- **THEN** a dramatic ascending melody plays during the evolution animation

#### Scenario: Error sound
- **WHEN** an action fails (no food, wrong PIN, insufficient points)
- **THEN** a short low-pitched buzz plays

### Requirement: Non-blocking playback
The system SHALL play buzzer tones without blocking the main render loop.

#### Scenario: Animation continues during sound
- **WHEN** a melody is playing
- **THEN** the display continues rendering frames without stuttering

### Requirement: Silence mode
The system SHALL support disabling buzzer output via a software toggle.

#### Scenario: Silent mode
- **WHEN** the buzzer is disabled via settings
- **THEN** no sound is produced for any event but all other behavior is unchanged
