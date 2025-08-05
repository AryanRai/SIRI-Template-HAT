/**
 * @file can_protocol.cpp
 * @brief CAN protocol utility functions implementation
 * @author SIRI Electrical Team
 * @date 2025
 */

#include "can_protocol.h"

uint32_t buildCANID(uint8_t priority, uint8_t sourceID, uint8_t targetID, uint8_t msgType) {
    // Build CAN ID from components
    uint32_t canID = 0;
    
    // Set priority (bits 28-24)
    canID |= ((uint32_t)priority & 0x1F) << 24;
    
    // Set source ID (bits 23-16)
    canID |= ((uint32_t)sourceID & 0xFF) << 16;
    
    // Set target ID (bits 15-8)
    canID |= ((uint32_t)targetID & 0xFF) << 8;
    
    // Set message type (bits 7-0)
    canID |= ((uint32_t)msgType & 0xFF);
    
    return canID;
}

void parseCANID(uint32_t canID, uint8_t* priority, uint8_t* sourceID, uint8_t* targetID, uint8_t* msgType) {
    // Parse CAN ID into components
    if (priority != nullptr) {
        *priority = (canID >> 24) & 0x1F;
    }
    
    if (sourceID != nullptr) {
        *sourceID = (canID >> 16) & 0xFF;
    }
    
    if (targetID != nullptr) {
        *targetID = (canID >> 8) & 0xFF;
    }
    
    if (msgType != nullptr) {
        *msgType = canID & 0xFF;
    }
}