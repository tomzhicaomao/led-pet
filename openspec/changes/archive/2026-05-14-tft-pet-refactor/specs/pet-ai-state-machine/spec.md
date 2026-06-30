## ADDED Requirements

### Requirement: Autonomous state transitions
The pet SHALL autonomously transition between behavioral states based on time, hunger level, and external events.

#### Scenario: Pet gets hungry
- **WHEN** 8 hours have elapsed since the last feed
- **THEN** the pet transitions to HUNGRY state and displays hunger behavior

#### Scenario: Pet sleeps at night
- **WHEN** the current hour (via NTP) is between 21:00 and 07:00
- **THEN** the pet transitions to SLEEPING state with dimmed display

#### Scenario: Pet wakes in the morning
- **WHEN** the current hour transitions to 07:00
- **THEN** the pet transitions from SLEEPING to IDLE with a wake-up animation

### Requirement: Feeding response
The system SHALL transition the pet to EATING state when the feed button is pressed and food is available.

#### Scenario: Successful feed
- **WHEN** the child presses the feed button and at least one food item exists in inventory
- **THEN** the pet transitions to EATING, eating animation plays, food inventory decrements by 1, and feedCount increments

#### Scenario: Feed with no food
- **WHEN** the child presses the feed button and inventory is empty
- **THEN** the pet remains in current state and a "no food" visual cue displays briefly

### Requirement: Reward interrupt
The system SHALL preempt the current pet state with a HAPPY celebration when a parent reward is received.

#### Scenario: Reward during idle
- **WHEN** a parent sends reward points while the pet is IDLE
- **THEN** the pet transitions to HAPPY, plays celebration animation, shows +N points, then returns to IDLE

#### Scenario: Reward during sleep
- **WHEN** a parent sends reward points while the pet is SLEEPING
- **THEN** the reward is queued and the celebration plays when the pet wakes up

### Requirement: IDLE sub-state roaming
When in IDLE state, the pet SHALL automatically roam the scene, switching between walking, sitting, and curiosity sub-states at random intervals of 30-120 seconds.

#### Scenario: Pet walks across scene
- **WHEN** the pet is in ROAMING sub-state
- **THEN** the pet sprite moves horizontally across the scene, with occasional pauses

#### Scenario: Pet sits and rests
- **WHEN** the roaming timer selects SITTING sub-state
- **THEN** the pet stops moving and plays idle animation for 30-60 seconds before roaming again

### Requirement: Evolution trigger
The system SHALL transition to EVOLVING state when an evolution stone is used and feedCount meets the current stage threshold.

#### Scenario: Evolution ready
- **WHEN** feedCount >= stage threshold AND evolution stone is used
- **THEN** the pet transitions to EVOLVING, plays evolution animation, increments stage, and resets feedCount

#### Scenario: Evolution not ready
- **WHEN** evolution stone is used but feedCount is below the threshold
- **THEN** the stone is returned to inventory, the pet shows a "not ready" cue, and no evolution occurs

### Requirement: NTP time synchronization
The system SHALL synchronize time via NTP on Wi-Fi connect and periodically thereafter.

#### Scenario: Initial time sync
- **WHEN** Wi-Fi connects successfully
- **THEN** the system fetches current time via NTP within 10 seconds

#### Scenario: Offline fallback
- **WHEN** NTP sync fails
- **THEN** the pet operates without day/night rhythm, staying in IDLE/HUNGRY states only
