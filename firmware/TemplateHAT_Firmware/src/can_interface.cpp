/**
 * @file can_interface.cpp
 * @brief CAN network interface implementation with SN65HVD230 driver
 * @author SIRI Electrical Team
 * @date 2025
 */

#include "../include/can_interface.h"
#include "../include/can_protocol.h"
#include "../include/hat_config.h"
#include "../include/hardware_map.h"
#include "../include/telemetry.h"
#include <FlexCAN_T4.h>

// FlexCAN instance for SN65HVD230 transceiver
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;

CANInterface::CANInterface() {
    messageCount = 0;
    errorCount = 0;
    lastHeartbeatTime = 0;
    initialized = false;
}

CANInterface::~CANInterface() {
    // Destructor implementation
}

bool CANInterface::initialize() {
    #if HAT_DEBUG_ENABLED
    Serial.println("Initializing CAN interface with SN65HVD230...");
    #endif
    
    // Initialize FlexCAN_T4 for SN65HVD230
    can1.begin();
    can1.setBaudRate(CAN_BAUDRATE);
    can1.setTX((FLEXCAN_PINS)PIN_CAN_TX);   // <-- add this (e.g., 23)
    can1.setRX((FLEXCAN_PINS)PIN_CAN_RX);
    can1.setMaxMB(16);  // Set maximum mailboxes
    can1.enableFIFO();
    can1.enableLoopBack();  // Enable FIFO for better message handling
    can1.enableFIFOInterrupt();
    
    // Setup CAN message filters
    setupCANFilters();
    
    // Reset statistics
    resetStatistics();
    
    initialized = true;
    
    #if HAT_DEBUG_ENABLED
    Serial.print("CAN interface initialized at ");
    Serial.print(CAN_BAUDRATE);
    Serial.println(" bps");
    #endif
    
    return true;
}

bool CANInterface::sendMessage(const CANMessage_t& message) {
    if (!initialized) {
        return false;
    }
    
    // Convert CANMessage_t to FlexCAN CAN_message_t
    CAN_message_t flexMsg;
    flexMsg.id = message.id;
    flexMsg.len = message.length;
    flexMsg.flags.extended = 1;  // Use extended CAN ID format
    
    // Copy data payload
    for (uint8_t i = 0; i < message.length && i < 8; i++) {
        flexMsg.buf[i] = message.data[i];
    }
    
    // Send message
    bool success = can1.write(flexMsg);
    
    if (success) {
        messageCount++;
        updateStatistics();
        
        #if HAT_DEBUG_ENABLED
        logCANMessage(message, true);
        #endif
        
        // Flash communication LED
        digitalWrite(PIN_LED_COMM, HIGH);
        delay(1);
        digitalWrite(PIN_LED_COMM, LOW);
    } else {
        errorCount++;
        handleCANError();
    }
    
    return success;
}

bool CANInterface::receiveMessage(CANMessage_t& message) {
    if (!initialized) {
        return false;
    }
    
    CAN_message_t flexMsg;
    
    if (can1.read(flexMsg)) {
        // Convert FlexCAN message to CANMessage_t
        message.id = flexMsg.id;
        message.length = flexMsg.len;
        message.timestamp = millis();
        
        // Copy data payload
        for (uint8_t i = 0; i < flexMsg.len && i < 8; i++) {
            message.data[i] = flexMsg.buf[i];
        }
        
        messageCount++;
        updateStatistics();
        
        #if HAT_DEBUG_ENABLED
        logCANMessage(message, false);
        #endif
        
        // Flash communication LED
        digitalWrite(PIN_LED_COMM, HIGH);
        delay(1);
        digitalWrite(PIN_LED_COMM, LOW);
        
        return true;
    }
    
    return false;
}

void CANInterface::processMessages() {
    CANMessage_t message;
    
    // Process all available messages
    while (receiveMessage(message)) {
        // Check if message is for this HAT
        if (isMessageForThisHAT(message)) {
            // Parse message type from CAN ID
            uint8_t priority, sourceID, targetID, msgType;
            parseCANID(message.id, &priority, &sourceID, &targetID, &msgType);
            
            // Route message to appropriate handler
            switch (msgType) {
                case MSG_TYPE_CONTROL_START:
                case MSG_TYPE_CONTROL_STOP:
                case MSG_TYPE_CONTROL_RESET:
                case MSG_TYPE_CONTROL_ENABLE:
                case MSG_TYPE_CONTROL_DISABLE:
                case MSG_TYPE_CONTROL_SET_PARAM:
                    handleControlMessage(message);
                    break;
                    
                case MSG_TYPE_STATUS_REQUEST:
                case MSG_TYPE_PARAM_REQUEST:
                case MSG_TYPE_DIAGNOSTIC_REQ:
                    handleStatusRequest(message);
                    break;
                    
                case MSG_TYPE_STATE_UNLOCK:
                case MSG_TYPE_STATE_DISARM:
                case MSG_TYPE_STATE_ARM:
                case MSG_TYPE_STATE_LOCK:
                case MSG_TYPE_STATE_EMERGENCY:
                case MSG_TYPE_STATE_RESET:
                case MSG_TYPE_STATE_QUERY:
                    handleStateCommand(message);
                    break;
                    
                case MSG_TYPE_CONFIG_SET:
                case MSG_TYPE_CONFIG_GET:
                case MSG_TYPE_CONFIG_SAVE:
                case MSG_TYPE_CONFIG_LOAD:
                    handleConfigMessage(message);
                    break;
                    
                case MSG_TYPE_EMERGENCY_STOP:
                case MSG_TYPE_EMERGENCY_BATTERY:
                case MSG_TYPE_EMERGENCY_THERMAL:
                case MSG_TYPE_EMERGENCY_COMM:
                case MSG_TYPE_SYSTEM_SHUTDOWN:
                    handleEmergencyMessage(message);
                    break;
                    
                default:
                    #if HAT_DEBUG_ENABLED
                    Serial.print("Unknown message type: 0x");
                    Serial.println(msgType, HEX);
                    #endif
                    break;
            }
        }
    }
}

bool CANInterface::sendHeartbeat() {
    CANMessage_t message;
    
    // Build heartbeat message
    message.id = buildCANID(CAN_PRIORITY_TEMPLATE, HAT_NODE_ID, CAN_BROADCAST_ADDR, MSG_TYPE_HEARTBEAT);
    message.length = 4;
    message.data[0] = HAT_NODE_ID;
    message.data[1] = 0x01; // Status: alive
    message.data[2] = (millis() >> 8) & 0xFF;  // Timestamp high byte
    message.data[3] = millis() & 0xFF;         // Timestamp low byte
    
    lastHeartbeatTime = millis();
    return sendMessage(message);
}

bool CANInterface::sendTelemetry(const TelemetryData_t& data) {
    CANMessage_t message;
    
    // Format telemetry message
    message.id = buildCANID(CAN_PRIORITY_TEMPLATE, HAT_NODE_ID, CAN_BROADCAST_ADDR, MSG_TYPE_TELEMETRY_SENSOR);
    message.length = 8;
    
    // Pack telemetry data into CAN message
    message.data[0] = data.sensorId & 0xFF;
    message.data[1] = (data.sensorId >> 8) & 0xFF;
    message.data[2] = (uint8_t)data.dataType;
    
    // Pack float value as bytes (assuming float data type)
    union {
        float f;
        uint8_t bytes[4];
    } valueUnion;
    valueUnion.f = data.data.floatValue;
    
    message.data[3] = valueUnion.bytes[0];
    message.data[4] = valueUnion.bytes[1];
    message.data[5] = valueUnion.bytes[2];
    message.data[6] = valueUnion.bytes[3];
    message.data[7] = data.timestamp & 0xFF;
    
    return sendMessage(message);
}

bool CANInterface::sendStateResponse(HAT_State_t state) {
    CANMessage_t message;
    
    message.id = buildCANID(CAN_PRIORITY_TEMPLATE, HAT_NODE_ID, CAN_BROADCAST_ADDR, MSG_TYPE_STATE_RESPONSE);
    message.length = 3;
    message.data[0] = HAT_NODE_ID;
    message.data[1] = (uint8_t)state;
    message.data[2] = millis() & 0xFF; // Timestamp
    
    return sendMessage(message);
}

bool CANInterface::sendEmergencyStop() {
    CANMessage_t message;
    
    message.id = buildCANID(CAN_PRIORITY_TEMPLATE, HAT_NODE_ID, CAN_BROADCAST_ADDR, MSG_TYPE_EMERGENCY_STOP);
    message.length = 2;
    message.data[0] = HAT_NODE_ID;
    message.data[1] = 0xFF; // Emergency code
    
    return sendMessage(message);
}

void CANInterface::handleControlMessage(const CANMessage_t& message) {
    #if HAT_DEBUG_ENABLED
    Serial.println("Handling control message");
    #endif
    
    // Extract message type
    uint8_t priority, sourceID, targetID, msgType;
    parseCANID(message.id, &priority, &sourceID, &targetID, &msgType);
    
    // Process control commands
    switch (msgType) {
        case MSG_TYPE_CONTROL_START:
            // Handle start command
            break;
        case MSG_TYPE_CONTROL_STOP:
            // Handle stop command
            break;
        case MSG_TYPE_CONTROL_RESET:
            // Handle reset command
            break;
        // Add other control message handlers
    }
}

void CANInterface::handleStatusRequest(const CANMessage_t& message) {
    #if HAT_DEBUG_ENABLED
    Serial.println("Handling status request");
    #endif
    
    // Send status response
    CANMessage_t response;
    response.id = buildCANID(CAN_PRIORITY_TEMPLATE, HAT_NODE_ID, CAN_BROADCAST_ADDR, MSG_TYPE_STATUS_RESPONSE);
    response.length = 4;
    response.data[0] = HAT_NODE_ID;
    response.data[1] = 0x01; // Status OK
    response.data[2] = messageCount & 0xFF;
    response.data[3] = errorCount & 0xFF;
    
    sendMessage(response);
}

void CANInterface::handleStateCommand(const CANMessage_t& message) {
    #if HAT_DEBUG_ENABLED
    Serial.println("Handling state command");
    #endif
    
    // Forward to state machine for processing
    // This would typically involve calling state machine methods
}

void CANInterface::handleConfigMessage(const CANMessage_t& message) {
    #if HAT_DEBUG_ENABLED
    Serial.println("Handling config message");
    #endif
    
    // Handle configuration messages
}

void CANInterface::handleEmergencyMessage(const CANMessage_t& message) {
    #if HAT_DEBUG_ENABLED
    Serial.println("EMERGENCY MESSAGE RECEIVED!");
    #endif
    
    // Handle emergency messages - immediate action required
    // Trigger emergency stop in state machine
}

bool CANInterface::isMessageForThisHAT(const CANMessage_t& message) {
    uint8_t priority, sourceID, targetID, msgType;
    parseCANID(message.id, &priority, &sourceID, &targetID, &msgType);
    
    // Message is for this HAT if:
    // 1. It's a broadcast message (targetID == 0xFF)
    // 2. It's specifically addressed to this HAT (targetID == HAT_NODE_ID)
    return (targetID == CAN_BROADCAST_ADDR || targetID == HAT_NODE_ID);
}

void CANInterface::logCANMessage(const CANMessage_t& message, bool outgoing) {
    #if HAT_DEBUG_ENABLED
    uint8_t priority, sourceID, targetID, msgType;
    parseCANID(message.id, &priority, &sourceID, &targetID, &msgType);
    
    Serial.print(outgoing ? "[TX] " : "[RX] ");
    Serial.print("ID: 0x");
    Serial.print(message.id, HEX);
    Serial.print(" P:");
    Serial.print(priority);
    Serial.print(" S:0x");
    Serial.print(sourceID, HEX);
    Serial.print(" T:0x");
    Serial.print(targetID, HEX);
    Serial.print(" MT:0x");
    Serial.print(msgType, HEX);
    Serial.print(" Data: ");
    
    for (uint8_t i = 0; i < message.length; i++) {
        Serial.print("0x");
        if (message.data[i] < 0x10) Serial.print("0");
        Serial.print(message.data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
    #endif
}

uint32_t CANInterface::getMessageCount() {
    return messageCount;
}

uint32_t CANInterface::getErrorCount() {
    return errorCount;
}

void CANInterface::resetStatistics() {
    messageCount = 0;
    errorCount = 0;
    lastHeartbeatTime = millis();
}

void CANInterface::setupCANFilters() {
    // Setup CAN filters to accept messages for this HAT
    // Filter for broadcast messages and messages specifically for this HAT
    
    // Accept all messages for now - can be optimized later
    can1.setFIFOFilter(ACCEPT_ALL);
}

void CANInterface::handleCANError() {
    #if HAT_DEBUG_ENABLED
    Serial.println("CAN Error occurred");
    #endif
    
    // Flash error LED
    digitalWrite(PIN_LED_ERROR, HIGH);
    delay(100);
    digitalWrite(PIN_LED_ERROR, LOW);
}

void CANInterface::updateStatistics() {
    // Update communication statistics
    // This could include calculating message rates, error rates, etc.
}