/**
 * @file state_machine.h
 * @brief HAT state machine interface and definitions
 * @author SIRI Electrical Team
 * @date 2025
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

// HAT State Definitions
typedef enum {
    STATE_POWER_OFF = 0,
    STATE_POWER_IDLE = 1,
    STATE_DISARMED = 2,
    STATE_UNLOCKED = 3,
    STATE_POWER_ARMED = 4,
    STATE_LOCKED = 5,
    STATE_EMERGENCY_STOP = 6
} HAT_State_t;

// Authority Levels
typedef enum {
    AUTHORITY_SYSTEM = 0,
    AUTHORITY_BASE_STATION = 1,
    AUTHORITY_OPERATOR = 2,
    AUTHORITY_BPS = 3,
    AUTHORITY_EMERGENCY = 4
} Authority_t;

// State Machine Events
typedef enum {
    EVENT_POWER_ON = 0,
    EVENT_POWER_OFF = 1,
    EVENT_UNLOCK = 2,
    EVENT_DISARM = 3,
    EVENT_ARM = 4,
    EVENT_LOCK = 5,
    EVENT_EMERGENCY = 6,
    EVENT_RESET = 7,
    EVENT_TIMEOUT = 8
} StateMachineEvent_t;

// State Machine Class
class HATStateMachine {
public:
    // Constructor/Destructor
    HATStateMachine();
    ~HATStateMachine();
    
    // Initialization
    bool initialize();
    
    // State Management
    HAT_State_t getCurrentState();
    bool transitionToState(HAT_State_t newState, Authority_t authority);
    bool processEvent(StateMachineEvent_t event, Authority_t authority);
    
    // Command Authorization
    bool isCommandAllowed(uint8_t commandType);
    bool validateAuthority(Authority_t authority, StateMachineEvent_t event);
    
    // Timeout Handling
    void handleTimeout();
    void resetTimeout();
    uint32_t getStateUptime();
    
    // Emergency Handling
    void handleEmergency();
    void enterSafeMode();
    
    // State Information
    const char* getStateName(HAT_State_t state);
    const char* getCurrentStateName();
    
    // Callbacks (virtual - to be overridden by specific HAT implementations)
    virtual void onEnterState(HAT_State_t state);
    virtual void onExitState(HAT_State_t state);
    virtual bool validateTransition(HAT_State_t from, HAT_State_t to);

private:
    HAT_State_t currentState;
    HAT_State_t previousState;
    uint32_t stateEntryTime;
    uint32_t lastActivityTime;
    bool emergencyFlag;
    
    // Internal helper functions
    bool isValidTransition(HAT_State_t from, HAT_State_t to);
    void updateStateTimestamp();
    void logStateTransition(HAT_State_t from, HAT_State_t to);
};

#endif // STATE_MACHINE_H