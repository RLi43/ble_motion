#ifndef _IMU_H_
#define _IMU_H_

#include "imu_bles.h"

extern void ble_update_acc(float*);
extern void ble_update_ori(float*);
extern void ble_update_acc_acc(uint8_t);

bool imu_init();

void imu_task();

#endif // _IMU_H_