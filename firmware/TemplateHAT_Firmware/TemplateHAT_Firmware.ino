/**
 * @file TemplateHAT_Firmware.ino
 * @brief Main Arduino sketch for TemplateHAT
 * @author SIRI Electrical Team
 * @date 2025
 * 
 * Hardware: Teensy 4.1 with TJA1050 CAN transceiver
 * IDE: Arduino IDE with Teensyduino add-on
 * 
 * Required Libraries:
 * - FlexCAN_T4 (install via Library Manager)
 * - ArduinoJson (install via Library Manager)
 */

#include "include/hat_config.h"
#include "include/can_interface.h"
#include "include/state_machine.h"
#include "include/component_ctrl.h"
#include "include/telemetry.h"
#include "include/hardware_map.h"

// Global objects
CANInterface canInterface;
HATStateMachine stateMachine;
ComponentController componentController;
TelemetryManager telemetryManager;

// Timing variables
unsigned long lastHeartbeat = 0;
unsigned long lastTelemetry = 0;
unsigned long lastStateCheck = 0;

void setup() {
    // Initialize serial communication
    Serial.begin(HAT_SERIAL_BAUD_RATE);
    delay(1000);
    
    #if HAT_DEBUG_ENABLED
    Serial.println("=== SIRI TemplateHAT Starting ===");
    Serial.print("HAT Name: ");
    Serial.println(HAT_NAME);
    Serial.print("Version: ");
    Serial.println(HAT_VERSION);
    Serial.print("Node ID: 0x");
    Serial.println(HAT_NODE_ID, HEX);
    #endif
    
    // Initialize hardware
    initializeHardware();
    
    // Initialize subsystems
    initializeSubsystems();
    
    #if HAT_DEBUG_ENABLED
    Serial.println("=== Initialization Complete ===");
    #endif
}

void loop() {
    unsigned long currentTime = millis();
    
    // Process CAN messages
    processCANMessages();
    
    // Update state machine
    updateStateMachine(currentTime);
    
    // Send heartbeat
    sendHeartbeat(currentTime);
    
    // Send telemetry
    sendTelemetry(currentTime);
    
    // Update components
    updateComponents();
    
    // Handle status indicators
    updateStatusIndicators();
    
    // Small delay to prevent overwhelming the system
    delay(1);
}

void initializeHardware() {
    // Initialize GPIO pins
    pinMode(PIN_LED_STATUS, OUTPUT);
    pinMode(PIN_LED_ERROR, OUTPUT);
    pinMode(PIN_LED_COMM, OUTPUT);
    pinMode(PIN_BUTTON_TEST, INPUT_PULLUP);
    
    // Initialize status LEDs
    digitalWrite(PIN_LED_STATUS, LOW);
    digitalWrite(PIN_LED_ERROR, LOW);
    digitalWrite(PIN_LED_COMM, LOW);
    
    // Initialize other hardware pins
    initializeComponentPins();
}

void initializeSubsystems() {
    // Initialize CAN interface
    if (!canInterface.initialize()) {
        #if HAT_DEBUG_ENABLED
        Serial.println("ERROR: CAN interface initialization failed");
        #endif
        // Handle initialization error
        handleInitializationError();
        return;
    }
    
    // Initialize state machine
    if (!stateMachine.initialize()) {
        #if HAT_DEBUG_ENABLED
        Serial.println("ERROR: State machine initialization failed");
        #endif
        handleInitializationError();
        return;
    }
    
    // Initialize component controller
    if (!componentController.initialize()) {
        #if HAT_DEBUG_ENABLED
        Serial.println("ERROR: Component controller initialization failed");
        #endif
        handleInitializationError();
        return;
    }
    
    // Initialize telemetry manager
    if (!telemetryManager.initialize()) {
        #if HAT_DEBUG_ENABLED
        Serial.println("ERROR: Telemetry manager initialization failed");
        #endif
        handleInitializationError();
        return;
    }
}

void processCANMessages() {
    // Process incoming CAN messages
    canInterface.processMessages();
}

void updateStateMachine(unsigned long currentTime) {
    // Check for state timeouts
    if (currentTime - lastStateCheck >= HAT_STATE_TIMEOUT_MS) {
        stateMachine.handleTimeout();
        lastStateCheck = currentTime;
    }
}

void sendHeartbeat(unsigned long currentTime) {
    if (currentTime - lastHeartbeat >= HAT_HEARTBEAT_INTERVAL_MS) {
        canInterface.sendHeartbeat();
        lastHeartbeat = currentTime;
    }
}

void sendTelemetry(unsigned long currentTime) {
    if (currentTime - lastTelemetry >= HAT_TELEMETRY_INTERVAL_MS) {
        telemetryManager.sendTelemetry();
        lastTelemetry = currentTime;
    }
}

void updateComponents() {
    // Update component states based on current HAT state
    componentController.update();
}

void updateStatusIndicators() {
    // Update status LEDs based on system state
    updateStatusLEDs();
}

void initializeComponentPins() {
    // Initialize component-specific pins
    // To be implemented based on specific HAT requirements
}

void handleInitializationError() {
    // Handle initialization errors
    digitalWrite(PIN_LED_ERROR, HIGH);
    // Additional error handling logic
}

void updateStatusLEDs() {
    // Update status LEDs based on current state
    HAT_State_t currentState = stateMachine.getCurrentState();
    
    switch (currentState) {
        case STATE_POWER_OFF:
            digitalWrite(PIN_LED_STATUS, LOW);
            break;
        case STATE_DISARMED:
            // Slow blink - default state, waiting for Jetson pings
            digitalWrite(PIN_LED_STATUS, (millis() / 1000) % 2);
            break;
        case STATE_UNLOCKED:
            // Fast blink
            digitalWrite(PIN_LED_STATUS, (millis() / 250) % 2);
            break;
        case STATE_POWER_ARMED:
            digitalWrite(PIN_LED_STATUS, HIGH);
            break;
        case STATE_LOCKED:
            digitalWrite(PIN_LED_STATUS, LOW);
            digitalWrite(PIN_LED_ERROR, HIGH);
            break;
        case STATE_EMERGENCY_STOP:
            // Rapid flash
            digitalWrite(PIN_LED_STATUS, (millis() / 50) % 2);
            digitalWrite(PIN_LED_ERROR, HIGH);
            break;
    }
}