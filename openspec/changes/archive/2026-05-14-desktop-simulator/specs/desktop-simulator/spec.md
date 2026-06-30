## ADDED Requirements

### Requirement: TFT display simulation
The system SHALL render a 320×240 pixel simulation of the TFT display in a tkinter Canvas at 3x zoom (960×720).

#### Scenario: Window opens
- **WHEN** `python simulator.py` is executed
- **THEN** a tkinter window appears with title "LED Pet Simulator" showing a 960×720 Canvas

#### Scenario: Background renders
- **WHEN** the simulator starts
- **THEN** the Canvas fills with the pet's background color (dark green)

### Requirement: Sprite rendering
The system SHALL parse RGB565 sprite arrays from C header files and render them on the canvas at specified (x, y) coordinates.

#### Scenario: Parse sprite header
- **WHEN** the simulator loads `src/pet/sprite_test.h`
- **THEN** the `test_sprite_dragon` array is extracted and converted to RGB tuples

#### Scenario: Render pet sprite
- **WHEN** a frame is rendered with pet at position (140, 120)
- **THEN** the 32×32 dragon sprite appears at the correct canvas position

### Requirement: Status bar
The system SHALL render a status bar showing pet name, stage, points, and current AI state.

#### Scenario: Status bar content
- **WHEN** the simulator is running
- **THEN** text like "Dragon Baby  STG:1  PTS:42" is visible at the top of the canvas

### Requirement: Keyboard input mapping
The system SHALL map keyboard keys to pet actions: 1=Feed, 2=Interact, 3=Back, R=Reward+5, F=QuickFeed.

#### Scenario: Feed via keyboard
- **WHEN** key "1" is pressed
- **THEN** the pet transitions to EATING state

#### Scenario: Reward via keyboard
- **WHEN** key "R" is pressed
- **THEN** the pet transitions to HAPPY state and points increase by 5

### Requirement: Game loop
The system SHALL update game logic and render at approximately 30fps using tkinter's `after()` event loop.

#### Scenario: Continuous rendering
- **WHEN** the simulator is running
- **THEN** the pet sprite moves and animates smoothly without manual refresh

### Requirement: Pet AI integration
The system SHALL integrate a Python PetAI class that mirrors the C++ PetAI state machine behavior (IDLE/HUNGRY/EATING/HAPPY/SLEEPING/EVOLVING).

#### Scenario: Pet gets hungry
- **WHEN** more than 8 simulated hours pass since last feed
- **THEN** the pet state transitions to HUNGRY

#### Scenario: Pet happiness on reward
- **WHEN** reward is sent
- **THEN** the pet shows HAPPY state with celebration particles for 3 seconds

### Requirement: Exit
The system SHALL close cleanly when the window is closed or Escape is pressed.

#### Scenario: Window close
- **WHEN** the user closes the tkinter window
- **THEN** the program exits with code 0
