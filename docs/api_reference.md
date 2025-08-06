# TemplateHAT API Reference

## Overview
This document provides a comprehensive API reference for the TemplateHAT firmware implementation.

## Core Classes

### HATStateMachine Class

#### Public Methods

##### `bool initialize()`
**Description**: Initialize the state machine
**Returns**: `true` if successful, `false` otherwise
**Usage**: Call once during system startup

##### `HAT_State_t getCurrentState()`
**Description**: Get the current state of the HAT
**Returns**: Current state enumeration value
**Usage**: Query current operational state

##### `bool transitionToState(HAT_State_t newState, Authority_t authority)`
**Description**: Request transition to a new state
**Parameters**:
- `newState`: Target state to transition to
- `authority`: Authority level of the requester
**Returns**: `true` if transition successful, `false` otherwise

##### `bool isCommandAllowed(uint8_t commandType)`
**Description**: Check if a command is allowed in the current state
**Parameters**:
- `commandType`: Command type to check
**Returns**: `true` if command is allowed, `false` otherwise

### CANInterface Class

#### Public Methods

##### `bool initialize()`
**Description**: Initialize the CAN interface
**Returns**: `true` if successful, `false` otherwise
**Usage**: Call once during system startup

##### `bool sendMessage(const CANMessage_t& message)`
**Description**: Send a CAN message
**Parameters**:
- `message`: CAN message structure to send
**Returns**: `true` if message sent successfully, `false` otherwise

##### `bool receiveMessage(CANMessage_t& message)`
**Description**: Receive a CAN message
**Parameters**:
- `message`: Reference to store received message
**Returns**: `true` if message received, `false` if no message available

##### `bool sendHeartbeat()`
**Description**: Send heartbeat message
**Returns**: `true` if heartbeat sent successfully, `false` otherwise
**Usage**: Call periodically to maintain network presence

### ComponentController Class

#### Public Methods

##### `bool initialize()`
**Description**: Initialize the component controller
**Returns**: `true` if successful, `false` otherwise
**Usage**: Call once during system startup

##### `bool setComponentState(uint16_t componentAddr, uint8_t state)`
**Description**: Set the state of a specific component
**Parameters**:
- `componentAddr`: Address of the component
- `state`: New state value
**Returns**: `true` if state set successfully, `false` otherwise

##### `uint8_t getComponentState(uint16_t componentAddr)`
**Description**: Get the current state of a component
**Parameters**:
- `componentAddr`: Address of the component
**Returns**: Current state value, or 0xFF if component not found

##### `void emergencyStop()`
**Description**: Immediately stop all components
**Usage**: Call in emergency situations to ensure safety

### TelemetryManager Class

#### Public Methods

##### `bool initialize()`
**Description**: Initialize the telemetry manager
**Returns**: `true` if successful, `false` otherwise
**Usage**: Call once during system startup

##### `void sendTelemetry()`
**Description**: Send current telemetry data
**Usage**: Call periodically to transmit system status

##### `bool addSensorData(uint16_t sensorId, float value, TelemetryDataType_t type)`
**Description**: Add sensor data to telemetry buffer
**Parameters**:
- `sensorId`: Unique identifier for the sensor
- `value`: Sensor reading value
- `type`: Data type enumeration
**Returns**: `true` if data added successfully, `false` if buffer full

## Data Structures

### CANMessage_t
```cpp
typedef struct {
    uint32_t id;           // Extended CAN ID
    uint8_t data[8];       // Data payload (0-8 bytes)
    uint8_t length;        // Data length (0-8)
    uint32_t timestamp;    // Message timestamp
} CANMessage_t;
```

### TelemetryData_t
```cpp
typedef struct {
    uint16_t sensorId;
    TelemetryDataType_t dataType;
    uint32_t timestamp;
    union {
        int32_t intValue;
        float floatValue;
        bool boolValue;
        char stringValue[8];
    } data;
} TelemetryData_t;
```

### ComponentStatus_t
```cpp
typedef struct {
    uint16_t address;
    uint8_t state;
    bool enabled;
    bool responding;
    uint32_t lastUpdate;
    float value;
    uint8_t errorCode;
} ComponentStatus_t;
```

## Enumerations

### HAT_State_t
```cpp
typedef enum {
    STATE_POWER_OFF = 0,
    STATE_DISARMED = 1,      // Initial state after boot
    STATE_LOCKED = 2,        // Default operational state
    STATE_UNLOCKED = 3,
    STATE_POWER_ARMED = 4,
    STATE_EMERGENCY_STOP = 5
} HAT_State_t;
```

### Authority_t
```cpp
typedef enum {
    AUTHORITY_SYSTEM = 0,
    AUTHORITY_BASE_STATION = 1,
    AUTHORITY_OPERATOR = 2,
    AUTHORITY_BPS = 3,
    AUTHORITY_EMERGENCY = 4
} Authority_t;
```

### TelemetryDataType_t
```cpp
typedef enum {
    TELEMETRY_TYPE_INT = 0,
    TELEMETRY_TYPE_FLOAT = 1,
    TELEMETRY_TYPE_BOOL = 2,
    TELEMETRY_TYPE_STRING = 3
} TelemetryDataType_t;
```

## Constants

### Message Types
```cpp
// Control Commands (0x00-0x0F)
#define MSG_TYPE_CONTROL_START      0x00
#define MSG_TYPE_CONTROL_STOP       0x01
#define MSG_TYPE_CONTROL_RESET      0x02

// State Machine Commands (0x30-0x3F)
#define MSG_TYPE_STATE_UNLOCK       0x30
#define MSG_TYPE_STATE_DISARM       0x31
#define MSG_TYPE_STATE_ARM          0x32

// Emergency Messages (0xF0-0xFF)
#define MSG_TYPE_EMERGENCY_STOP     0xF0
#define MSG_TYPE_SYSTEM_SHUTDOWN    0xFF
```

### Configuration Constants
```cpp
#define HAT_HEARTBEAT_INTERVAL_MS   1000
#define HAT_TELEMETRY_INTERVAL_MS   100
#define HAT_STATE_TIMEOUT_MS        5000
#define HAT_MAX_COMPONENTS          16
```

## Utility Functions

### CAN Protocol Functions

##### `uint32_t buildCANID(uint8_t priority, uint8_t sourceID, uint8_t targetID, uint8_t msgType)`
**Description**: Build a CAN ID from components
**Parameters**:
- `priority`: Message priority (1-7)
- `sourceID`: Source node ID
- `targetID`: Target node ID (0xFF for broadcast)
- `msgType`: Message type
**Returns**: Constructed 29-bit CAN ID

##### `void parseCANID(uint32_t canID, uint8_t* priority, uint8_t* sourceID, uint8_t* targetID, uint8_t* msgType)`
**Description**: Parse a CAN ID into components
**Parameters**:
- `canID`: 29-bit CAN ID to parse
- `priority`: Pointer to store priority
- `sourceID`: Pointer to store source ID
- `targetID`: Pointer to store target ID
- `msgType`: Pointer to store message type

## Error Codes

### System Error Codes
```cpp
#define ERROR_NONE              0x00
#define ERROR_COMM_TIMEOUT      0x01
#define ERROR_INVALID_COMMAND   0x02
#define ERROR_HARDWARE_FAULT    0x03
#define ERROR_SENSOR_FAILURE    0x04
#define ERROR_ACTUATOR_FAULT    0x05
#define ERROR_POWER_ISSUE       0x06
#define ERROR_THERMAL_LIMIT     0x07
#define ERROR_EMERGENCY_STOP    0x08
```

## Usage Examples

### Basic Initialization
```cpp
// Initialize all subsystems
canInterface.initialize();
stateMachine.initialize();
componentController.initialize();
telemetryManager.initialize();
```

### Sending a CAN Message
```cpp
CANMessage_t message;
message.id = buildCANID(HAT_PRIORITY, HAT_NODE_ID, 0x01, MSG_TYPE_STATUS_RESPONSE);
message.data[0] = stateMachine.getCurrentState();
message.length = 1;
canInterface.sendMessage(message);
```

### State Transition
```cpp
if (stateMachine.transitionToState(STATE_POWER_ARMED, AUTHORITY_OPERATOR)) {
    // Transition successful
    componentController.enableComponent(ADDR_COMPONENT_1);
} else {
    // Transition failed
    Serial.println("State transition denied");
}
```

### Adding Telemetry Data
```cpp
float temperature = 25.5;
telemetryManager.addSensorData(0x001, temperature, TELEMETRY_TYPE_FLOAT);
```