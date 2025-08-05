/**
 * @file hat_config.h
 * @brief HAT-specific configuration definitions
 * @author SIRI Electrical Team
 * @date 2025
 */

#ifndef HAT_CONFIG_H
#define HAT_CONFIG_H

#include <stdint.h>

// HAT Identification
#define HAT_NAME "TemplateHAT"
#define HAT_VERSION "1.0.0"
#define HAT_NODE_ID 0x07  // Template HAT Node ID
#define HAT_BASE_ADDRESS 0x700  // Template HAT base address

// Hardware Configuration
#define HAT_MCU_TYPE "Teensy 4.1"
#define HAT_CAN_TRANSCEIVER "TJA1050"
#define HAT_POWER_VOLTAGE_5V 5.0f
#define HAT_POWER_VOLTAGE_3V3 3.3f

// Component Configuration
#define HAT_MAX_COMPONENTS 16
#define HAT_COMPONENT_BASE_ADDR (HAT_BASE_ADDRESS + 0x10)

// Timing Configuration
#define HAT_HEARTBEAT_INTERVAL_MS 1000
#define HAT_TELEMETRY_INTERVAL_MS 100
#define HAT_STATE_TIMEOUT_MS 5000

// Debug Configuration
#define HAT_DEBUG_ENABLED 1
#define HAT_SERIAL_BAUD_RATE 115200

#endif // HAT_CONFIG_H