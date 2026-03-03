/**
 * @file TemplateHAT_Firmware.ino
 * @brief Main Arduino sketch for TemplateHAT
 * @author SIRI Electrical Team
 * @date 2025
 * 
 * Hardware: Teensy 4.1 with SN65HVD230 CAN transceiver
 * IDE: Arduino IDE with Teensyduino add-on
 * 
 * Required Libraries:
 * - FlexCAN_T4 (install via Library Manager)
 * - ArduinoJson (install via Library Manager)
 */

#include "include/hat_config.h"
#include "include/hardware_map.h"
#include "include/can_protocol.h"
#include "include/state_machine.h"
#include "include/telemetry.h"
#include "include/can_interface.h"
#include "include/component_ctrl.h"
#include <FlexCAN_T4.h>
#include "include/sirius_can_id_map.h"
#include <SPI.h>
#include <MFRC522.h>
#include <math.h>  // for logf
// Telemetry message types
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
static inline float wrap180(float deg) {
  while (deg <= -180.0f) deg += 360.0f;
  while (deg >   180.0f) deg -= 360.0f;
  return deg;
}

static inline void pack_i16(int16_t v, uint8_t* p) {
  p[0] = (uint8_t)(v & 0xFF);
  p[1] = (uint8_t)((v >> 8) & 0xFF);
}

static inline void send_vec3_as_i16(CANInterface& can,
                                    uint32_t id,
                                    float x, float y, float z,
                                    float scale) {
  int16_t xi = (int16_t)roundf(x * scale);
  int16_t yi = (int16_t)roundf(y * scale);
  int16_t zi = (int16_t)roundf(z * scale);

  CANMessage_t msg{};
  msg.id = id;
  msg.length = 6;
  pack_i16(xi, &msg.data[0]);
  pack_i16(yi, &msg.data[2]);
  pack_i16(zi, &msg.data[4]);
  can.sendMessage(msg);
}

// Chosen scales (keep receiver consistent)
#define SCALE_EULER   100.0f   // deg -> centi-deg
#define SCALE_ACCEL  1000.0f   // m/s^2 -> milli-(m/s^2)
#define SCALE_GYRO   1000.0f   // rad/s -> milli-(rad/s)
#define SCALE_LINACC 1000.0f   // m/s^2 -> milli-(m/s^2)

// Use CAN message type definitions from can_protocol.h
static float readThermC() {
  int raw = analogRead(HAT_ANALOG_TEMP);           // 0..4095
  float v = raw * (HAT_ADC_VREF/ 4095.0f);        // convert to volts

  // Guards: if miswired (rail), avoid divide-by-zero/log of 0
  if (v <= 0.0001f||v >= (HAT_ADC_VREF-0.0001f)) return NAN;

  
  float r_ntc = HAT_THERM_R_FIXED * v / (HAT_ADC_VREF - v);
  const float T0 = 298.15f;                          // 25°C in Kelvin
  float invT = (1.0f / T0) + (1.0f / HAT_THERM_BETA) * logf(r_ntc / HAT_THERM_R0);
  float T = 1.0f / invT;                             // Kelvin
  return T - 273.15f;                                // Celsiu
}



// Global objects
CANInterface canInterface;
HATStateMachine stateMachine;
ComponentController componentController;
TelemetryManager telemetryManager;
MFRC522 rfid(HAT_SPI_CS, HAT_RFID_RST);
Adafruit_BNO055 bno(55, 0x28);

// Timing variables
unsigned long lastHeartbeat = 0;
unsigned long lastTelemetry = 0;
unsigned long lastStateCheck = 0;

void setup() {
    // Initialize serial communication
    Serial.begin(HAT_SERIAL_BAUD_RATE);
    analogReadResolution(12);
    analogReadAveraging(8);

    pinMode(HAT_SPI_CS, OUTPUT);
    digitalWrite(HAT_SPI_CS, HIGH);  // deselect MFRC522

    
  // rfid.PCD_DumpVersionToSerial(); // optional verbose

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
    Wire.begin();
if (!bno.begin()) {
  Serial.println("BNO055 not found (check wiring / address 0x28/0x29)");
} else {
  bno.setExtCrystalUse(true);    // better heading stability
  Serial.println("BNO055 ready");
}

    // Initialize hardware
    initializeHardware();

    // Initialize subsystems
    initializeSubsystems();

    #if HAT_DEBUG_ENABLED
    Serial.println("=== Initialization Complete ===");
    #endif
    // optional for single-node test
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

    // ---- TEMP TEST BLOCK
static uint32_t lastTemp = 0;
if (millis() - lastTemp >= 200) {     // ~10 Hz
  lastTemp = millis();

  float c = readThermC();
  if (!isnan(c)) {
    int16_t cx100 = (int16_t)roundf(c * 100.0f);

    // Build your own protocol message (no FlexCAN types here)
    CANMessage_t out{};
    out.id = buildCANID(CAN_PRIORITY_TEMPLATE, HAT_NODE_ID,
                        CAN_BROADCAST_ADDR, MSG_TYPE_TELEMETRY_SENSOR);
    out.length = 2;
    out.data[0] = (uint8_t)(cx100 & 0xFF);
    out.data[1] = (uint8_t)((cx100 >> 8) & 0xFF);

    canInterface.sendMessage(out);   // <-- let CANInterface talk to FlexCAN

    #if HAT_DEBUG_ENABLED
      Serial.printf("Therm: %.2f C\n", c);
    #endif
    digitalWrite(PIN_LED_COMM, HIGH); delayMicroseconds(200); digitalWrite(PIN_LED_COMM, LOW);
  } else {
    #if HAT_DEBUG_ENABLED
      Serial.println("Therm: NAN (check wiring)");
    #endif
  }
}
// --- Helper: check MFRC522 is online (reads VersionReg) ---

// ---- RFID POLL (10 Hz), non-blocking, no early returns ----

// ---- bno BLOCK (BNO055): YPR + raw accel/gyro + linacc @ ~50 Hz ----
static uint32_t lastIMU = 0;
if (millis() - lastIMU >= 200) {   
  lastIMU = millis();

  // 1) Fused orientation (Euler)
  sensors_event_t e;
  bno.getEvent(&e);  // degrees

  float yaw_deg   = wrap180(e.orientation.heading); 
  float pitch_deg = e.orientation.pitch;
  float roll_deg  = e.orientation.roll;

  {
    int16_t yaw_x100   = (int16_t)roundf(yaw_deg   * SCALE_EULER);
    int16_t pitch_x100 = (int16_t)roundf(pitch_deg * SCALE_EULER);
    int16_t roll_x100  = (int16_t)roundf(roll_deg  * SCALE_EULER);

    CANMessage_t out{};
    out.id = ID_IMU_EULER; // 0x21
    out.length = 6;
    pack_i16(yaw_x100,   &out.data[0]);
    pack_i16(pitch_x100, &out.data[2]);
    pack_i16(roll_x100,  &out.data[4]);
    canInterface.sendMessage(out);

    #if HAT_DEBUG_ENABLED
      Serial.printf("IMU Euler (deg): yaw=%.2f pitch=%.2f roll=%.2f\n",
                    yaw_deg, pitch_deg, roll_deg);
    #endif
  }

  // 2) Raw accelerometer (m/s^2) 
  {
    imu::Vector<3> a = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    uint32_t id = ID_IMU_ACCEL; // 0x25
    send_vec3_as_i16(canInterface, id, a.x(), a.y(), a.z(), SCALE_ACCEL);
    #if HAT_DEBUG_ENABLED
      Serial.printf("IMU Acc (m/s^2): %.3f %.3f %.3f\n", a.x(), a.y(), a.z());
    #endif
  }

  // 3) Raw gyroscope (rad/s) 
  {
    imu::Vector<3> g = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    uint32_t id = ID_IMU_GYRO; // 0x26
    send_vec3_as_i16(canInterface, id, g.x(), g.y(), g.z(), SCALE_GYRO);
    #if HAT_DEBUG_ENABLED
      Serial.printf("IMU Gyro (rad/s): %.3f %.3f %.3f\n", g.x(), g.y(), g.z());
    #endif
  }

  // 4) Linear acceleration (m/s^2, gravity removed) 
  {
    imu::Vector<3> la = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    uint32_t id = ID_IMU_LINACC; // 0x27
    send_vec3_as_i16(canInterface, id, la.x(), la.y(), la.z(), SCALE_LINACC);
    #if HAT_DEBUG_ENABLED
      Serial.printf("IMU LinAcc (m/s^2): %.3f %.3f %.3f\n", la.x(), la.y(), la.z());
    #endif
  }
}
static uint32_t lastCal = 0;
if (millis() - lastCal >= 500) {   // 2 Hz
  lastCal = millis();
  uint8_t sys, gyro, accel, mag;
  bno.getCalibration(&sys, &gyro, &accel, &mag);

  CANMessage_t cal{};
  cal.id =ID_IMU_CALIB; // 0x23 (already defined)
  cal.length = 4;
  cal.data[0] = sys; cal.data[1] = gyro; cal.data[2] = accel; cal.data[3] = mag;
  canInterface.sendMessage(cal);
}






    
  
    updateComponents();
    

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
