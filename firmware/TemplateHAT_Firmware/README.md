# TemplateHAT Arduino Firmware

## Overview
This is the Arduino IDE version of the TemplateHAT firmware for SIRI rover HAT modules.

## Hardware Requirements
- **Microcontroller**: Teensy 4.1
- **CAN Transceiver**: TJA1050
- **Power Supply**: 5V from backplane
- **Connections**: 40-pin HAT connector

## Software Requirements
- **Arduino IDE**: Version 1.8.19 or later
- **Teensyduino**: Latest version for Teensy 4.1 support
- **Required Libraries**:
  - FlexCAN_T4 (install via Library Manager)
  - ArduinoJson (install via Library Manager)

## Installation

### 1. Install Arduino IDE and Teensyduino
1. Download and install Arduino IDE from https://www.arduino.cc/
2. Download and install Teensyduino from https://www.pjrc.com/teensy/teensyduino.html
3. Follow the installation instructions for your operating system

### 2. Install Required Libraries
1. Open Arduino IDE
2. Go to **Tools > Manage Libraries**
3. Search for and install:
   - **FlexCAN_T4** by tonton81
   - **ArduinoJson** by Benoit Blanchon

### 3. Configure Arduino IDE
1. Go to **Tools > Board** and select **Teensy 4.1**
2. Set **Tools > USB Type** to **Serial**
3. Set **Tools > CPU Speed** to **600 MHz**
4. Set **Tools > Optimize** to **Faster**

## Project Structure

### Main Files
- **TemplateHAT_Firmware.ino** - Main sketch with setup() and loop()
- **README.md** - This documentation file

### Header Files (include/)
- **hat_config.h** - HAT-specific configuration constants
- **can_protocol.h** - CAN message definitions and protocol
- **hardware_map.h** - Pin mappings and hardware configuration
- **can_interface.h** - CAN interface class declarations
- **state_machine.h** - State machine class and enumerations
- **component_ctrl.h** - Component controller class
- **telemetry.h** - Telemetry manager class

### Source Files (src/)
- **can_interface.cpp** - CAN network communication implementation
- **state_machine.cpp** - HAT state machine implementation
- **component_ctrl.cpp** - Component control and monitoring
- **telemetry.cpp** - Telemetry data collection and transmission
- **can_protocol.cpp** - CAN protocol utility functions

## Usage

### 1. Open the Project
1. Open Arduino IDE
2. Open **TemplateHAT_Firmware.ino**
3. All other files should automatically load as tabs

### 2. Customize for Your HAT
1. Edit **hat_config.h** to set your HAT's node ID and base address
2. Modify **hardware_map.h** for your specific pin assignments
3. Implement HAT-specific functionality in the .ino files

### 3. Upload to Teensy
1. Connect Teensy 4.1 via USB
2. Select the correct COM port in **Tools > Port**
3. Click **Upload** or press **Ctrl+U**

## Configuration

### HAT Identification
Edit these values in **include/hat_config.h**:
```cpp
#define HAT_NAME "YourHATName"
#define HAT_NODE_ID 0x07        // Your assigned node ID
#define HAT_BASE_ADDRESS 0x700  // Your assigned base address
```

### Pin Assignments
Modify **include/hardware_map.h** for your hardware:
```cpp
// Example: Change LED pins
#define PIN_LED_STATUS 13
#define PIN_LED_ERROR 12
#define PIN_LED_COMM 11
```

### CAN Network Settings
The CAN network is configured for:
- **Baud Rate**: 1 Mbps
- **TX Pin**: 22 (CAN1_TX)
- **RX Pin**: 23 (CAN1_RX)

## Development

### Adding New Components
1. Define component addresses in **hardware_map.h**
2. Add component control logic in **component_ctrl.ino**
3. Update telemetry collection in **telemetry.ino**

### State Machine Customization
1. Override virtual functions in **state_machine.ino**:
   - `onEnterState()`
   - `onExitState()`
   - `validateTransition()`

### CAN Message Handling
1. Add message handlers in **can_interface.ino**
2. Define new message types in **can_protocol.h**
3. Implement protocol logic in **can_protocol.ino**

## Debugging

### Serial Monitor
- Open **Tools > Serial Monitor**
- Set baud rate to **115200**
- Debug messages will appear when `HAT_DEBUG_ENABLED` is set to 1

### LED Status Indicators
- **Status LED (Pin 13)**: Shows current state
  - Off: POWER_OFF
  - Slow blink: POWER_IDLE
  - Fast blink: DISARMED
  - Double blink: UNLOCKED
  - Solid: POWER_ARMED
  - Rapid flash: EMERGENCY_STOP
- **Error LED (Pin 12)**: Shows error conditions
- **Comm LED (Pin 11)**: Shows CAN communication activity

## Testing

### Basic Functionality Test
1. Upload firmware to Teensy
2. Open Serial Monitor
3. Look for initialization messages
4. Check LED patterns for state indication

### CAN Network Test
1. Connect to CAN bus with proper termination
2. Use CAN analyzer or another HAT to send test messages
3. Monitor serial output for message reception
4. Verify heartbeat transmission

## Troubleshooting

### Common Issues
1. **Compilation Errors**:
   - Ensure all required libraries are installed
   - Check that Teensyduino is properly installed
   - Verify board selection is Teensy 4.1

2. **Upload Failures**:
   - Press the program button on Teensy during upload
   - Check USB cable connection
   - Verify correct COM port selection

3. **CAN Communication Issues**:
   - Check CAN bus wiring (CANH, CANL, GND)
   - Verify 120Ω termination resistors
   - Confirm baud rate matches network (1 Mbps)

4. **No Serial Output**:
   - Check baud rate setting (115200)
   - Ensure `HAT_DEBUG_ENABLED` is set to 1
   - Verify USB connection and drivers

### Getting Help
- Check the main SIRI-Elec documentation
- Review the technical API documentation
- Test with the provided examples
- Use the CAN network analyzer tools

## License
This project is part of the SIRI-Elec system and follows the same licensing terms.