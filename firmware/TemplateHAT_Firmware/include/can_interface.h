/**
 * @file can_interface.h
 * @brief CAN network interface header
 * @author SIRI Electrical Team
 * @date 2025
 */

#ifndef CAN_INTERFACE_H
#define CAN_INTERFACE_H

#include <stdint.h>
#include "can_protocol.h"
#include "state_machine.h"

// Forward declarations
struct TelemetryData_t;

class CANInterface {
public:
    // Constructor/Destructor
    CANInterface();
    ~CANInterface();
    
    // Initialization
    bool initialize();
    
    // Message Transmission
    bool sendMessage(const CANMessage_t& message);
    bool sendHeartbeat();
    bool sendTelemetry(const TelemetryData_t& data);
    bool sendStateResponse(HAT_State_t state);
    bool sendEmergencyStop();
    
    // Message Reception
    bool receiveMessage(CANMessage_t& message);
    void processMessages();
    
    // Message Handlers
    void handleControlMessage(const CANMessage_t& message);
    void handleStatusRequest(const CANMessage_t& message);
    void handleStateCommand(const CANMessage_t& message);
    void handleConfigMessage(const CANMessage_t& message);
    void handleEmergencyMessage(const CANMessage_t& message);
    
    // Utility Functions
    bool isMessageForThisHAT(const CANMessage_t& message);
    void logCANMessage(const CANMessage_t& message, bool outgoing = false);
    
    // Statistics
    uint32_t getMessageCount();
    uint32_t getErrorCount();
    void resetStatistics();

private:
    uint32_t messageCount;
    uint32_t errorCount;
    uint32_t lastHeartbeatTime;
    bool initialized;
    
    // Internal helper functions
    void setupCANFilters();
    void handleCANError();
    void updateStatistics();
};

#endif // CAN_INTERFACE_H