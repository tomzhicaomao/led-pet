## ADDED Requirements

### Requirement: Points wallet
The system SHALL maintain a points balance that increases via parent rewards and decreases via food purchases.

#### Scenario: Points increase from reward
- **WHEN** a parent sends +5 points via the web console with correct PIN
- **THEN** the points balance increases by 5 immediately

#### Scenario: Points decrease from purchase
- **WHEN** food is purchased from the shop
- **THEN** the points balance decreases by the item price

#### Scenario: Insufficient points
- **WHEN** a purchase is attempted with points below the item price
- **THEN** the purchase is rejected and points remain unchanged

### Requirement: Food inventory
The system SHALL maintain an inventory of 5 food types, each with a maximum stack of 99 items.

#### Scenario: Food count increases on purchase
- **WHEN** food type 0 (apple) is purchased
- **THEN** foodBag[0] increments by 1

#### Scenario: Food count decreases on feed
- **WHEN** food type 1 (drumstick) is used for feeding
- **THEN** foodBag[1] decrements by 1

#### Scenario: Stack limit enforcement
- **WHEN** foodBag for any type is at 99 and more of that type is purchased
- **THEN** the purchase is rejected

### Requirement: Food shop
The system SHALL offer 5 food items with distinct prices, feed values, and types (4 normal + 1 evolution stone).

#### Scenario: Shop item properties
- **WHEN** the food config is initialized
- **THEN** items have prices (3, 5, 8, 12, 20) and feed values (1, 2, 3, 5, 0) respectively, with item 4 marked as evolution stone

#### Scenario: Normal food feeding
- **WHEN** a normal food item (types 0-3) is fed
- **THEN** feedCount increases by the food's feed value and lastFeedTime updates

#### Scenario: Evolution stone feeding (ready)
- **WHEN** food type 4 (evolution stone) is fed AND feedCount >= current stage threshold
- **THEN** currentStage increments, feedCount resets to 0, and evolved flag is set

#### Scenario: Evolution stone feeding (not ready)
- **WHEN** food type 4 (evolution stone) is fed AND feedCount < current stage threshold
- **THEN** the stone is returned to inventory and no evolution occurs

### Requirement: Evolution stages
The system SHALL define 5 evolution stages with ascending feed count thresholds.

#### Scenario: Stage thresholds
- **WHEN** checking evolution eligibility
- **THEN** stage 0 requires 3 feedCount, stage 1 requires 8, stage 2 requires 15, stage 3 requires 25, and stage 4 cannot evolve further

#### Scenario: Final stage cannot evolve
- **WHEN** the pet is at stage 4 (final) and an evolution stone is used
- **THEN** the operation fails with no effect

### Requirement: Hunger tracking
The system SHALL track time since last feed and report hunger status.

#### Scenario: Recently fed
- **WHEN** last feed was within the past 8 hours
- **THEN** isHungry() returns false

#### Scenario: Hunger threshold exceeded
- **WHEN** last feed was more than 8 hours ago
- **THEN** isHungry() returns true

### Requirement: PIN-protected reward
The system SHALL require a 4-digit PIN match to add points via the parent API.

#### Scenario: Correct PIN
- **WHEN** addPoints is called with a matching PIN
- **THEN** points are added and the call returns true

#### Scenario: Incorrect PIN
- **WHEN** addPoints is called with a non-matching PIN
- **THEN** points are unchanged and the call returns false
