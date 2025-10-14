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

// ---- CAN (Teensy 4.x CAN1) ----
#define PIN_CAN_TX 23          // CTX1  (FIXED)
#define PIN_CAN_RX 22          // CRX1  (FIXED)

// ---- Status LEDs & button (move off SPI pins) ----
#define PIN_LED_STATUS 2
#define PIN_LED_ERROR  3
#define PIN_LED_COMM   4
#define PIN_BUTTON_TEST 5

// ---- I2C (default bus) ----
#define HAT_I2C_SDA 18
#define HAT_I2C_SCL 19

// ---- SPI (default bus) ----
#define HAT_SPI_MOSI 11
#define HAT_SPI_MISO 12
#define HAT_SPI_SCK  13
#define HAT_SPI_CS   10        // use 10 as CS for RFID
#define HAT_RFID_RST 9         // add RST for RFID

// ---- Analog inputs ----
#define HAT_ANALOG_VOLTAGE A0  // 14
#define HAT_ANALOG_CURRENT A2  // 16
#define HAT_ANALOG_TEMP    A1  // 15  (thermistor node)

// ---- Example digital IO (free pins) ----
#define HAT_DIGITAL_OUT_1 20
#define HAT_DIGITAL_OUT_2 21
#define HAT_DIGITAL_IN_1  6
#define HAT_DIGITAL_IN_2  7

// ---- PWM examples (free pins) ----
#define HAT_PWM_OUT_1 8
#define HAT_PWM_OUT_2 9
#define HAT_PWM_OUT_3 14
#define HAT_PWM_OUT_4 15

// ---- CAN addressing (keep your template values) ----
#define ADDR_STATE_QUERY      (HAT_BASE_ADDRESS + 0xF0)
#define ADDR_STATE_RESPONSE   (HAT_BASE_ADDRESS + 0xF1)
#define ADDR_STATE_TRANSITION (HAT_BASE_ADDRESS + 0xF2)
#define ADDR_EMERGENCY_STOP   (HAT_BASE_ADDRESS + 0xF3)
#define ADDR_AUTHORITY_CHECK  (HAT_BASE_ADDRESS + 0xF4)
#define ADDR_TIMEOUT_CONFIG   (HAT_BASE_ADDRESS + 0xF5)

#define ADDR_COMPONENT_1 (HAT_COMPONENT_BASE_ADDR + 0x00)
#define ADDR_COMPONENT_2 (HAT_COMPONENT_BASE_ADDR + 0x01)
#define ADDR_COMPONENT_3 (HAT_COMPONENT_BASE_ADDR + 0x02)
#define ADDR_COMPONENT_4 (HAT_COMPONENT_BASE_ADDR + 0x03)

// ---- Config structs (unchanged) ----
typedef struct { uint8_t pin; bool inverted; uint32_t address; const char* name; } ComponentConfig_t;
typedef struct { uint8_t txPin; uint8_t rxPin; uint32_t baudrate; uint8_t nodeID; } CANConfig_t;
typedef struct { uint8_t statusLED; uint8_t errorLED; uint8_t commLED; uint8_t testButton; } StatusConfig_t;

static const CANConfig_t CAN_CONFIG = { PIN_CAN_TX, PIN_CAN_RX, CAN_BAUDRATE, HAT_NODE_ID };
static const StatusConfig_t STATUS_CONFIG = { PIN_LED_STATUS, PIN_LED_ERROR, PIN_LED_COMM, PIN_BUTTON_TEST };

static const ComponentConfig_t COMPONENT_CONFIGS[] = {
  {HAT_DIGITAL_OUT_1, false, ADDR_COMPONENT_1, "Component 1"},
  {HAT_DIGITAL_OUT_2, false, ADDR_COMPONENT_2, "Component 2"},

};
#endif
