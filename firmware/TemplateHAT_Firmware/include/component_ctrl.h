/**
 * @file component_ctrl.h
 * @brief Component controller header
 * @author SIRI Electrical Team
 * @date 2025
 */

#ifndef COMPONENT_CTRL_H
#define COMPONENT_CTRL_H

#include <stdint.h>
#include "hardware_map.h"

// Component Status Structure
typedef struct {
    uint16_t address;
    uint8_t state;
    bool enabled;
    bool responding;
    uint32_t lastUpdate;
    float value;
    uint8_t errorCode;
} ComponentStatus_t;

class ComponentController {
public:
    // Constructor/Destructor
    ComponentController();
    ~ComponentController();
    
    // Initialization
    bool initialize();
    void update();
    
    // Component Control
    bool setComponentState(uint16_t componentAddr, uint8_t state);
    uint8_t getComponentState(uint16_t componentAddr);
    bool enableComponent(uint16_t componentAddr);
    bool disableComponent(uint16_t componentAddr);
    
    // Component Configuration
    bool configureComponent(uint16_t componentAddr, const ComponentConfig_t& config);
    bool calibrateComponent(uint16_t componentAddr);
    bool testComponent(uint16_t componentAddr);
    
    // Safety Functions
    void emergencyStop();
    void enterSafeMode();
    
    // Status and Monitoring
    ComponentStatus_t getComponentStatus(uint16_t componentAddr);
    void getAllComponentStatus(ComponentStatus_t* statusArray, uint8_t maxCount);
    uint8_t getComponentCount();
    bool isComponentResponding(uint16_t componentAddr);
    
    // Maintenance
    void resetComponent(uint16_t componentAddr);
    void resetAllComponents();

private:
    ComponentStatus_t components[HAT_MAX_COMPONENTS];
    uint8_t componentCount;
    bool initialized;
    
    // Internal helper functions
    int8_t findComponentIndex(uint16_t componentAddr);
    void updateComponentStatus(uint8_t index);
    void initializeComponentHardware();
};

#endif // COMPONENT_CTRL_H