#include "imu.h"

#include "BNO08x.hpp"

float imu_acc[3];
uint8_t imu_acc_acc = 0;
float imu_ori[3];
bool operating = false;

bno08x_config_t imu_config(
    SPI2_HOST,
    GPIO_NUM_21,
    GPIO_NUM_5,
    GPIO_NUM_4,
    GPIO_NUM_7,
    GPIO_NUM_2,
    GPIO_NUM_3,
    GPIO_NUM_NC,
    2000000UL,
    false
);
// // imu_config.io_wake = GPIO_NUM_10;
// // imu_config.debug_en = true;
// imu_config.io_mosi = GPIO_NUM_21; //assign pin
// imu_config.io_miso = GPIO_NUM_5; //assign pin
// imu_config.io_sclk = GPIO_NUM_4;
// imu_config.io_cs   = GPIO_NUM_7;
// imu_config.io_int  = GPIO_NUM_2;
// imu_config.io_rst  = GPIO_NUM_3;
static BNO08x imu = BNO08x(imu_config);
bool imu_init()
{
    bool ret;
    
    // imu = BNO08x(imu_config); //pass config to BNO08x constructor

    ret = imu.initialize();
    
    if(ret){
        //enable gyro & game rotation vector
        imu.enable_linear_accelerometer(200000UL); //150,000us == 150ms report interval 
        imu.enable_rotation_vector(200000UL); //100,000us == 100ms report interval

        operating = true;
    }

    return ret;
}

void imu_task()
{
    if(operating && imu.data_available())
    {
        imu.get_linear_accel(imu_acc[0], imu_acc[1], imu_acc[2], imu_acc_acc);
        imu.get_rpy_deg(imu_ori[0], imu_ori[1], imu_ori[2]);

        ble_update_acc(imu_acc);
        ble_update_acc_acc(imu_acc_acc);
        ble_update_ori(imu_ori);

        // ESP_LOGW("IMU", "Linear Acceleration(acc=%u): x: %.3f y: %.3f z: %.3f", acc_accuracy, acc_x, acc_y, acc_z);
        // ESP_LOGI("IMU", "Oritentation(deg): x (roll): %.3f y (pitch): %.3f z (yaw): %.3f", ori_x, ori_y, ori_z);
    }
}