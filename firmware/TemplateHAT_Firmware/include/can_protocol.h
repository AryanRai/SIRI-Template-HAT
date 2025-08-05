/**
 * @file can_protocol.h
 * @brief CAN message definitions and protocol constants
 * @author SIRI Electrical Team
 * @date 2025
 */

#ifndef CAN_PROTOCOL_H
#define CAN_PROTOCOL_H

#include <stdint.h>

// CAN Network Configuration
#define CAN_BAUDRATE 1000000  // 1 Mbps
#define CAN_MAX_NODES 32
#define CAN_MAX_DATA_LENGTH 8

// Extended CAN ID Structure (29-bit)
// Bits 28-24: Priority (5 bits)
// Bits 23-16: Source Node ID (8 bits)
// Bits 15-8:  Target Node ID (8 bits) - 0xFF for broadcast
// Bits 7-0:   Message Type/Register (8 bits)

// Priority Levels
#define CAN_PRIORITY_JETSON 1
#define CAN_PRIORITY_DRIVEHAT 2
#define CAN_PRIORITY_BPS 3
#define CAN_PRIORITY_ARMHAT 4
#define CAN_PRIORITY_SCIHAT 5
#define CAN_PRIORITY_SENSEHAT 6
#define CAN_PRIORITY_TEMPLATE 7

// Broadcast Address
#define CAN_BROADCAST_ADDR 0xFF

// Message Types - Control Commands (0x00-0x0F)
#define MSG_TYPE_CONTROL_START 0x00
#define MSG_TYPE_CONTROL_STOP 0x01
#define MSG_TYPE_CONTROL_RESET 0x02
#define MSG_TYPE_CONTROL_ENABLE 0x03
#define MSG_TYPE_CONTROL_DISABLE 0x04
#define MSG_TYPE_CONTROL_SET_PARAM 0x05

// Message Types - Status Requests (0x10-0x1F)
#define MSG_TYPE_STATUS_REQUEST 0x10
#define MSG_TYPE_STATUS_RESPONSE 0x11
#define MSG_TYPE_PARAM_REQUEST 0x12
#define MSG_TYPE_PARAM_RESPONSE 0x13
#define MSG_TYPE_DIAGNOSTIC_REQ 0x14
#define MSG_TYPE_DIAGNOSTIC_RESP 0x15

// Message Types - Telemetry Data (0x20-0x2F)
#define MSG_TYPE_TELEMETRY_SENSOR 0x20
#define MSG_TYPE_TELEMETRY_STATUS 0x21
#define MSG_TYPE_TELEMETRY_ERROR 0x22
#define MSG_TYPE_HEARTBEAT 0x23

// Message Types - State Machine Commands (0x30-0x3F)
#define MSG_TYPE_STATE_UNLOCK 0x30
#define MSG_TYPE_STATE_DISARM 0x31
#define MSG_TYPE_STATE_ARM 0x32
#define MSG_TYPE_STATE_LOCK 0x33
#define MSG_TYPE_STATE_EMERGENCY 0x34
#define MSG_TYPE_STATE_RESET 0x35
#define MSG_TYPE_STATE_QUERY 0x36
#define MSG_TYPE_STATE_RESPONSE 0x37

// Message Types - Configuration (0x40-0x4F)
#define MSG_TYPE_CONFIG_SET 0x40
#define MSG_TYPE_CONFIG_GET 0x41
#define MSG_TYPE_CONFIG_SAVE 0x42
#define MSG_TYPE_CONFIG_LOAD 0x43

// Message Types - Emergency/System Messages (0xF0-0xFF)
#define MSG_TYPE_EMERGENCY_STOP 0xF0
#define MSG_TYPE_EMERGENCY_BATTERY 0xF1
#define MSG_TYPE_EMERGENCY_THERMAL 0xF2
#define MSG_TYPE_EMERGENCY_COMM 0xF3
#define MSG_TYPE_SYSTEM_SHUTDOWN 0xFF

// CAN Message Structure
typedef struct {
    uint32_t id;           // Extended CAN ID
    uint8_t data[8];       // Data payload (0-8 bytes)
    uint8_t length;        // Data length (0-8)
    uint32_t timestamp;    // Message timestamp
} CANMessage_t;

// Function prototypes
uint32_t buildCANID(uint8_t priority, uint8_t sourceID, uint8_t targetID, uint8_t msgType);
void parseCANID(uint32_t canID, uint8_t* priority, uint8_t* sourceID, uint8_t* targetID, uint8_t* msgType);

#endif // CAN_PROTOCOL_H