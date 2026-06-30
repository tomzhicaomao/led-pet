## ADDED Requirements

### Requirement: Parent reward distribution
The system SHALL provide a web page where a parent can send reward points to the device after PIN authentication.

#### Scenario: Send reward with reason
- **WHEN** the parent selects a reward amount and a reason tag, and submits with correct PIN
- **THEN** the device adds the points, displays a celebration animation, and logs the reward with reason

#### Scenario: Send reward with wrong PIN
- **WHEN** the parent submits a reward with incorrect PIN
- **THEN** the request is rejected with an error message

### Requirement: Pet status display
The web page SHALL display the current pet status including species name, evolution stage, points, and mood.

#### Scenario: Status page load
- **WHEN** the parent opens the web page
- **THEN** the current pet name, stage, points balance, and mood are displayed

#### Scenario: Status auto-refresh
- **WHEN** the web page is open
- **THEN** pet status refreshes every 5 seconds via polling

### Requirement: Reward history
The system SHALL maintain an in-memory ring buffer of the last 20 reward events and display them on the web page.

#### Scenario: Recent rewards visible
- **WHEN** the parent views the web page
- **THEN** the last several rewards are listed with timestamp, amount, and reason

### Requirement: PIN change
The web page SHALL allow the parent to change the 4-digit PIN with old PIN verification.

#### Scenario: Successful PIN change
- **WHEN** the parent provides the correct old PIN and a new 4-digit PIN
- **THEN** the PIN is updated and persisted

#### Scenario: Failed PIN change
- **WHEN** the old PIN does not match
- **THEN** the PIN is unchanged and an error is returned

### Requirement: CORS support for dev proxy
The HTTP server SHALL include CORS headers to allow local development with a Vite dev proxy.

#### Scenario: CORS preflight
- **WHEN** the browser sends an OPTIONS request from a different origin
- **THEN** the server responds with Access-Control-Allow-Origin: * and appropriate method headers

### Requirement: SPIFFS static file serving
The system SHALL serve the parent web console HTML, CSS, and JS files from SPIFFS.

#### Scenario: Root path serves index.html
- **WHEN** a browser requests the device root URL
- **THEN** the parent management page HTML is served with text/html content type

#### Scenario: Missing file returns 404
- **WHEN** a requested static file does not exist in SPIFFS
- **THEN** the server returns HTTP 404
