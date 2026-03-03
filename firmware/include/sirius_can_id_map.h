#pragma once
#include <stdint.h>

// Match sirius_can_map.hpp
#define PRIORITY_SENSE   (0x6u << 8)   // 0x600

// Use your existing message types as "message_id"
#define ID_HEARTBEAT     (PRIORITY_SENSE | 0x23)

#define ID_IMU_ACCEL     (PRIORITY_SENSE | 0x25)
#define ID_IMU_GYRO      (PRIORITY_SENSE | 0x26)
#define ID_IMU_LINACC    (PRIORITY_SENSE | 0x27)
#define ID_IMU_EULER     (PRIORITY_SENSE | 0x2A)
#define ID_IMU_CALIB     (PRIORITY_SENSE | 0x2B)

// (Optional) temperature
#define ID_TEMP          (PRIORITY_SENSE | 0x20)

