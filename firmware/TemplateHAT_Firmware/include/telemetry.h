/**
 * @file telemetry.h
 * @brief Telemetry manager header
 * @author SIRI Electrical Team
 * @date 2025
 */

#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stdint.h>
#include "can_protocol.h"

// Telemetry Data Types
typedef enum {
    TELEMETRY_TYPE_INT = 0,
    TELEMETRY_TYPE_FLOAT = 1,
    TELEMETRY_TYPE_BOOL = 2,
    TELEMETRY_TYPE_STRING = 3
} TelemetryDataType_t;

// Telemetry Data Structure
typedef struct {
    uint16_t sensorId;
    TelemetryDataType_t dataType;
    uint32_t timestamp;
    union {
        int32_t intValue;
        float floatValue;
        bool boolValue;
        char stringValue[8];
    } data;
} TelemetryData_t;

class TelemetryManager {
public:
    // Constructor/Destructor
    TelemetryManager();
    ~TelemetryManager();
    
    // Initialization
    bool initialize();
    
    // Telemetry Transmission
    void sendTelemetry();
    
    // Data Collection
    bool addSensorData(uint16_t sensorId, float value, TelemetryDataType_t type);
    bool addStatusData(uint8_t statusCode, const char* message);
    bool addErrorData(uint8_t errorCode, const char* description);
    
    // Configuration
    void setTelemetryInterval(uint32_t intervalMs);
    uint32_t getTelemetryInterval();
    void enableTelemetry(bool enable);
    bool isTelemetryEnabled();
    
    // Data Access
    TelemetryData_t getCurrentTelemetryData();
    void clearTelemetryData();
    
    // Statistics
    uint32_t getTelemetryCount();
    void resetTelemetryCount();
    
    // Logging
    bool logTelemetryData(const TelemetryData_t& data);

private:
    TelemetryData_t telemetryBuffer[16];
    uint8_t bufferIndex;
    uint32_t telemetryInterval;
    uint32_t lastTelemetryTime;
    uint32_t telemetryCount;
    bool telemetryEnabled;
    bool initialized;
    
    // Internal helper functions
    void updateSystemTelemetry();
    void updateComponentTelemetry();
    void formatTelemetryMessage(const TelemetryData_t& data, CANMessage_t& message);
};

#endif // TELEMETRY_H