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
├── hardware/                 # PCB design files
│   ├── schematic/           # KiCad schematic files
│   │   ├── template_hat.kicad_sch
│   │   └── template_hat.kicad_pro
│   ├── pcb/                 # PCB layout files
│   │   ├── template_hat.kicad_pcb
│   │   └── gerbers/         # Manufacturing files
│   ├── 3d_models/           # 3D component models
│   └── bom/                 # Bill of Materials
│       ├── template_hat_bom.csv
│       └── component_specs.md
├── firmware/                # Microcontroller code
│   ├── src/                 # Source code
│   │   ├── main.cpp         # Main application
│   │   ├── can_interface.cpp # CAN network handler
│   │   ├── state_machine.cpp # HAT state machine
│   │   ├── component_ctrl.cpp # Hardware control
│   │   └── telemetry.cpp    # Data reporting
│   ├── include/             # Header files
│   │   ├── hat_config.h     # HAT-specific configuration
│   │   ├── can_protocol.h   # CAN message definitions
│   │   ├── state_machine.h  # State machine interface
│   │   └── hardware_map.h   # Pin and address mappings
│   ├── lib/                 # External libraries
│   │   ├── SIRI_CAN/        # SIRI CAN protocol library
│   │   └── StateMachine/    # State machine library
│   ├── test/                # Unit tests
│   │   ├── test_can.cpp     # CAN interface tests
│   │   ├── test_states.cpp  # State machine tests
│   │   └── test_hardware.cpp # Hardware tests
│   ├── platformio.ini       # PlatformIO configuration
│   └── CMakeLists.txt       # CMake build configuration
├── software/                # High-level software integration
│   ├── ros2/                # ROS2 integration
│   │   ├── package.xml      # ROS2 package definition
│   │   ├── setup.py         # Python package setup
│   │   └── template_hat/    # ROS2 node implementation
│   │       ├── __init__.py
│   │       ├── hat_node.py  # Main ROS2 node
│   │       └── hat_interface.py # HAT communication
│   └── python/              # Python utilities
│       ├── hat_simulator.py # HAT simulation for testing
│       ├── can_monitor.py   # CAN network monitoring
│       └── config_tool.py   # Configuration utility
├── docs/                    # Documentation
│   ├── design_specs.md      # Design specifications
│   ├── api_reference.md     # API documentation
│   ├── testing_guide.md     # Testing procedures
│   ├── assembly_guide.md    # Hardware assembly
│   └── troubleshooting.md   # Common issues and solutions
├── testing/                 # Testing and validation
│   ├── hardware_tests/      # Hardware validation
│   │   ├── power_test.py    # Power consumption tests
│   │   ├── can_test.py      # CAN interface tests
│   │   └── component_test.py # Component functionality
│   ├── integration_tests/   # System integration
│   │   ├── state_machine_test.py # State transitions
│   │   ├── network_test.py  # CAN network integration
│   │   └── mission_test.py  # Mission scenario tests
│   └── simulation/          # Virtual testing
│       ├── hat_model.py     # HAT simulation model
│       └── test_scenarios.py # Test case definitions
├── examples/                # Usage examples
│   ├── basic_hat/           # Minimal HAT implementation
│   ├── sensor_hat/          # Sensor-based HAT example
│   └── actuator_hat/        # Actuator control example
└── tools/                   # Development tools
    ├── address_generator.py # CAN address assignment
    ├── config_validator.py  # Configuration validation
    └── deployment_script.sh # Automated deployment
```

## Key Components

### Hardware Layer
- **Schematic Design**: KiCad-based PCB design with standard 40-pin HAT connector
- **Component Selection**: Standardized components (TJA1050 CAN transceiver for Teensy, SN65HVD230 for Jetson, etc.)
- **Power Management**: 5V/3.3V power distribution and monitoring
- **Status Indicators**: LEDs for state indication and debugging

### Firmware Layer
- **CAN Interface**: Implementation of SIRI CAN protocol
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

### 3. Firmware Development
- Implement HAT-specific component control
- Customize state machine behavior
- Add telemetry for HAT sensors/actuators

### 4. Integration Testing
- Test CAN network communication
- Validate state machine transitions
- Verify mission scenario compatibility

### 5. Documentation
- Update API documentation
- Create assembly and testing guides
- Document troubleshooting procedures

## Getting Started

### Prerequisites
- KiCad 7.0+ for hardware design
- PlatformIO or Arduino IDE for firmware
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

3. **Build firmware**:
   ```bash
   cd firmware
   platformio run
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
- **Hardware Interface**: 40-pin connector and power specifications
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
