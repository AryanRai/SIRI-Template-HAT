# SIRI-Template-HAT
A standardized template for creating HAT modules for the SIRI rover electrical system

## Overview

The TemplateHAT provides a standardized foundation for developing new HAT (Hardware Attached on Top) modules for the SIRI rover. This template implements the common hardware interface, CAN network protocols, and state machine requirements that all HATs must follow.

## Purpose

This template serves as:
- **Starting point** for new HAT development
- **Reference implementation** of SIRI-Elec standards
- **Testing framework** for CAN network integration
- **Documentation example** for HAT-specific features

## Project Structure

```
TemplateHAT/
├── README.md                 # This documentation
├── LICENSE                   # MIT License
├── firmware/                 # Arduino IDE firmware
│   └── TemplateHAT_Firmware/ # Arduino sketch folder
│       ├── TemplateHAT_Firmware.ino # Main Arduino sketch
│       ├── README.md         # Arduino-specific documentation
│       ├── include/          # Header files
│       │   ├── hat_config.h  # HAT configuration
│       │   ├── can_protocol.h # CAN message definitions
│       │   ├── state_machine.h # State machine interface
│       │   ├── hardware_map.h # Pin and address mappings
│       │   ├── can_interface.h # CAN interface declarations
│       │   ├── component_ctrl.h # Component controller
│       │   └── telemetry.h   # Telemetry manager
│       └── src/              # Source files
│           ├── can_interface.cpp # CAN implementation
│           ├── state_machine.cpp # State machine implementation
│           ├── component_ctrl.cpp # Component control
│           ├── telemetry.cpp # Telemetry implementation
│           └── can_protocol.cpp # CAN utility functions
├── docs/                     # Documentation
│   ├── design_specs.md       # Design specifications
│   └── api_reference.md      # API documentation
├── hardware/                 # PCB design files (future)
│   ├── schematic/           # KiCad schematic files
│   ├── pcb/                 # PCB layout files
│   ├── 3d_models/           # 3D component models
│   └── bom/                 # Bill of Materials
├── software/                 # High-level software integration (future)
│   ├── ros2/                # ROS2 integration
│   └── python/              # Python utilities
├── testing/                  # Testing and validation (future)
│   ├── hardware_tests/      # Hardware validation
│   ├── integration_tests/   # System integration
│   └── simulation/          # Virtual testing
├── examples/                 # Usage examples (future)
│   ├── basic_hat/           # Minimal HAT implementation
│   ├── sensor_hat/          # Sensor-based HAT example
│   └── actuator_hat/        # Actuator control example
└── tools/                    # Development tools (future)
    ├── address_generator.py # CAN address assignment
    ├── config_validator.py  # Configuration validation
    └── deployment_script.sh # Automated deployment
```

## Key Components

### Hardware Layer
- **Schematic Design**: KiCad-based PCB design with standard VGA HAT connector
- **Component Selection**: Standardized components (TJA1050 CAN transceiver for Teensy, SN65HVD230 for Jetson, etc.)
- **Power Management**: 5V/3.3V power distribution and monitoring
- **Status Indicators**: LEDs for state indication and debugging

### Firmware Layer (Arduino IDE)
- **Main Sketch**: TemplateHAT_Firmware.ino with setup() and loop()
- **CAN Interface**: Implementation of SIRI CAN protocol using FlexCAN_T4
- **State Machine**: Standardized safety states (POWER_OFF, IDLE, ARMED, etc.)
- **Component Control**: Abstract interface for HAT-specific hardware
- **Telemetry System**: Periodic status and sensor data reporting

### Software Integration
- **ROS2 Node**: High-level interface for mission control
- **Python Utilities**: Development and testing tools
- **Simulation Framework**: Virtual HAT for testing without hardware

### Documentation
- **Design Specifications**: Technical requirements and constraints
- **API Reference**: Complete interface documentation
- **Testing Procedures**: Validation and quality assurance
- **Assembly Guide**: Hardware construction instructions

## Development Workflow

### 1. HAT Specification
- Define HAT purpose and requirements
- Assign CAN addresses from [hw_inf_mappings.md](../hw_inf_mappings.md)
- Document component specifications

### 2. Hardware Design
- Modify schematic for HAT-specific components
- Update PCB layout maintaining standard connector
- Generate BOM and manufacturing files

### 3. Arduino Firmware Development
- Open TemplateHAT_Firmware.ino in Arduino IDE
- Install required libraries (FlexCAN_T4, ArduinoJson)
- Implement HAT-specific component control in .ino tabs
- Customize state machine behavior
- Add telemetry for HAT sensors/actuators

### 4. Integration Testing
- Upload firmware to Teensy 4.1
- Test CAN network communication
- Validate state machine transitions using serial monitor
- Verify mission scenario compatibility

### 5. Documentation
- Update API documentation
- Create assembly and testing guides
- Document troubleshooting procedures

## Getting Started

### Prerequisites
- KiCad 7.0+ for hardware design
- Arduino IDE with Teensyduino for firmware development
- Python 3.8+ for software tools
- ROS2 Humble for system integration

### Quick Start
1. **Clone the template**:
   ```bash
   git clone https://github.com/your-org/SIRI-Template-HAT.git MyNewHAT
   cd MyNewHAT
   ```

2. **Customize configuration**:
   - Edit `firmware/include/hat_config.h` with your HAT details
   - Update CAN addresses in `firmware/include/hardware_map.h`
   - Modify component definitions as needed

3. **Open firmware in Arduino IDE**:
   ```bash
   # Open TemplateHAT_Firmware.ino in Arduino IDE
   # Install required libraries: FlexCAN_T4, ArduinoJson
   # Select Board: Teensy 4.1
   # Upload to hardware
   ```

4. **Run tests**:
   ```bash
   cd testing
   python -m pytest hardware_tests/
   ```

## Standards Compliance

This template ensures compliance with:
- **SIRI-Elec CAN Protocol**: Message formats and addressing
- **State Machine Requirements**: Safety states and transitions
- **Hardware Interface**: VGA connector and power specifications
- **Documentation Standards**: Consistent documentation format

## Support

- **Main Documentation**: [SIRI-Elec README](../README.md)
- **Technical APIs**: [docs.md](../docs.md)
- **Hardware Mappings**: [hw_inf_mappings.md](../hw_inf_mappings.md)
- **State Machine**: [state_machine.md](../state_machine.md)

## Contributing

When contributing to TemplateHAT:
1. Follow the established project structure
2. Maintain compatibility with SIRI-Elec standards
3. Update documentation for any changes
4. Test thoroughly before submitting PRs

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
