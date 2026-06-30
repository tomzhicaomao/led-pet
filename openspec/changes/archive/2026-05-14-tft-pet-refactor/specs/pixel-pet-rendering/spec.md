## ADDED Requirements

### Requirement: Multi-species pet registry
The system SHALL maintain a registry of available pet species, each with its own sprite data for all evolution stages and animation states.

#### Scenario: List available pets
- **WHEN** the device boots
- **THEN** the pet registry provides a count of available species and can return any species by index

#### Scenario: Add new pet species
- **WHEN** a new species source file is added and the registry is updated
- **THEN** the new species is available without modifying any other files

### Requirement: Stage-based sprite selection
Each pet species SHALL provide distinct sprites for each of its 5 evolution stages, with visually obvious differences between stages.

#### Scenario: Sprite changes on evolution
- **WHEN** a pet evolves from stage 2 to stage 3
- **THEN** the rendered sprite visibly changes to the stage 3 variant

### Requirement: Animation state sprites
Each pet stage SHALL provide sprites for idle, eating, happy, and sleeping animation states.

#### Scenario: Eating animation
- **WHEN** the pet enters the EATING state
- **THEN** the eating sprite frames animate in sequence and loop until the state transitions

#### Scenario: Sleeping animation
- **WHEN** the pet enters the SLEEPING state
- **THEN** a minimal breathing animation plays (subtle scale or brightness pulse)

### Requirement: Pet position on screen
The system SHALL track and render the pet at a variable (x, y) position within the scene bounds.

#### Scenario: Pet moves across scene
- **WHEN** the pet is in ROAMING sub-state
- **THEN** the pet sprite position changes smoothly over time, staying within scene boundaries

### Requirement: Evolution visual transition
The system SHALL play a distinct visual sequence when evolution occurs.

#### Scenario: Evolution sequence
- **WHEN** the pet evolves
- **THEN** a flash or sparkle effect plays for 2-3 seconds, then the new stage sprite appears

### Requirement: Sprite data format
All sprites SHALL be stored as const uint16_t arrays in PROGMEM using RGB565 color format, generated from PNG source files.

#### Scenario: Sprite memory usage
- **WHEN** a pet species with 5 stages x 4 animations x 2 frames each, at 32x32 pixels is loaded
- **THEN** sprite data consumes approximately 80KB of flash (PROGMEM) and zero bytes of DRAM
