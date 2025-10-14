
- 0x0707FF23 Heartbeat: `[nodeId, 0x01, ts_hi, ts_lo]`.
- 0x0707FF20 Temperature: int16 LE → degC = raw/100.
- 0x0707FF2A IMU Euler: 3×int16 LE → deg = raw/100 (yaw, pitch, roll).
- 0x0707FF25 IMU Accel: 3×int16 LE → m/s² = raw/1000 (x,y,z).
- 0x0707FF26 IMU Gyro: 3×int16 LE → rad/s = raw/1000 (x,y,z).
- 0x0707FF27 IMU LinAcc: 3×int16 LE → m/s² = raw/1000 (x,y,z).
- 0x0707FF2B IMU Calib: 4×u8 → `sys, gyro, accel, mag` (0..3).
- 0x0707FF24 RFID UID: up to 8 raw bytes.



