/**
 * @file hardware_map.h
 * @brief Pin and address mappings for HAT hardware
 * @author SIRI Electrical Team
 * @date 2025
 */

#ifndef HARDWARE_MAP_H
#define HARDWARE_MAP_H

#include <stdint.h>
#include "hat_config.h"

// GPIO Pin Definitions (Teensy 4.1)
#define PIN_CAN_TX 22
#define PIN_CAN_RX 23
#define PIN_LED_STATUS 13
#define PIN_LED_ERROR 12
#define PIN_LED_COMM 11
#define PIN_BUTTON_TEST 10

// I2C Pins
#define PIN_I2C_SDA 18
#define PIN_I2C_SCL 19

// SPI Pins
#define PIN_SPI_MOSI 26
#define PIN_SPI_MISO 1
#define PIN_SPI_SCK 27
#define PIN_SPI_CS 0

// Analog Input Pins
#define PIN_ANALOG_VOLTAGE 14
#define PIN_ANALOG_CURRENT 15
#define PIN_ANALOG_TEMP 16

// Digital I/O Pins
#define PIN_DIGITAL_OUT_1 2
#define PIN_DIGITAL_OUT_2 3
#define PIN_DIGITAL_IN_1 4
#define PIN_DIGITAL_IN_2 5

// PWM Output Pins
#define PIN_PWM_OUT_1 6
#define PIN_PWM_OUT_2 7
#define PIN_PWM_OUT_3 8
#define PIN_PWM_OUT_4 9

// CAN Address Mappings
#define ADDR_STATE_QUERY (HAT_BASE_ADDRESS + 0xF0)
#define ADDR_STATE_RESPONSE (HAT_BASE_ADDRESS + 0xF1)
#define ADDR_STATE_TRANSITION (HAT_BASE_ADDRESS + 0xF2)
#define ADDR_EMERGENCY_STOP (HAT_BASE_ADDRESS + 0xF3)
#define ADDR_AUTHORITY_CHECK (HAT_BASE_ADDRESS + 0xF4)
#define ADDR_TIMEOUT_CONFIG (HAT_BASE_ADDRESS + 0xF5)

// Component Address Mappings (Template - to be customized per HAT)
#define ADDR_COMPONENT_1 (HAT_COMPONENT_BASE_ADDR + 0x00)
#define ADDR_COMPONENT_2 (HAT_COMPONENT_BASE_ADDR + 0x01)
#define ADDR_COMPONENT_3 (HAT_COMPONENT_BASE_ADDR + 0x02)
#define ADDR_COMPONENT_4 (HAT_COMPONENT_BASE_ADDR + 0x03)

// Hardware Configuration Structures
typedef struct {
    uint8_t pin;
    bool inverted;
    uint32_t address;
    const char* name;
} ComponentConfig_t;

typedef struct {
    uint8_t txPin;
    uint8_t rxPin;
    uint32_t baudrate;
    uint8_t nodeID;
} CANConfig_t;

typedef struct {
    uint8_t statusLED;
    uint8_t errorLED;
    uint8_t commLED;
    uint8_t testButton;
} StatusConfig_t;

// Hardware Configuration Constants
extern const CANConfig_t CAN_CONFIG;
extern const StatusConfig_t STATUS_CONFIG;
extern const ComponentConfig_t COMPONENT_CONFIGS[];

#endif // HARDWARE_MAP_H