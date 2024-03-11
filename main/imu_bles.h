// This file defines the IMU Service

#ifndef _IMU_BLES_H_
#define _IMU_BLES_H_

#include "ble_service.h"

#include "esp_gap_ble_api.h"
#include "esp_gatt_defs.h"

// Customized UUID
#define UUID_SERVICE_IMU    0xFF00
#define UUID_SERVICE_IMU_128 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00
#define UUID_CHRC_ACC       0xFF01
#define UUID_CHRC_ACAC      0xFF02
#define UUID_CHRC_ORI       0xFF10
#define UUID_CHRC_ORME      0xFF11
#define UUID_DESP_OPME_STR  0xFF12
#define UUID_CHRC_OPR       0xFF20

/*
 * BLE Specification
 *
 * UUID - Descriptor for GATT
 * Characteristic User Description      0x2901 
 *      UTF-8
 * Client Characteristic Configuration  0x2902      2 octets
 *      BIT0=1 - Notification, BIT1=1 - Indication, All others - Disabled
 * Characteristic Presentation Format   0x2904      7 octets
 *      Format, Exponent, Unit, Namespace, Description
 *      sb,     /       ,   sb, 0x01(SIG), 0x0000(No)
 * 
 * Valid Range             0x2911
 *      lower bound, then upper bound
 * 
 * Format Types
 * float32  0x14
 * float64  0x15
 * uint2    0x02
 * struct   0x18    opaque structure
 * 
 * UUID - Unit
 * acceleration (metres per second squared)         0x2713
 * angular acceleration (radian per second squared) 0x2744
 * angular velocity (radian per second)             0x2743
 * magnetic flux density (tesla)                    0x272D
 * velocity (metres per second)                     0x2712
 * plane angle (degree)                             0x2763
 * 
 */

/*
 * The IMU Service Structure
 *  Service: IMU Service
 *      Characteristic: Linear Acceleration
 *          Value: acc_x, acc_y, acc_z(3 4-byte floats)
 *          Descriptor: User Description: Linear Acceleration
 *          Descriptor: Client Characteristic Configuration
 *          Descriptor: Characteristic Presentation Format 
 *                  (float32: 0x14, acceleration m/s2: 0x2713)
 *      Characteristic: Linear Acceleration Accuracy
 *          Value: 2-bit Unsigned Int. 0, 1, 2
 *          Descriptor: User Description: Linear Acceleration Accuracy
 *          // Descriptor: Characteristic Presentation Format 
 * 
 *      Characteristic: Orientation
 *          Value: ori_x, ori_y, ori_z(3 4-byte floats)
 *          Descriptor: User Description: Orientation
 *          Descriptor: Client Characteristic Configuration
 *          Descriptor: Characteristic Presentation Format (float32: 0x14, plane angle(degree): 0x2763)
 *      Characteristic: Orientation Method
 *          Value: Unsigned 4bit    
 *              0-Geomagnetic   accelerometer + magnetometer
 *              1-Game          accelerometer + gyroscope
 *              2-VR/AR Stabilized Game     acc used to correct gyro
 *              3-""            accelerometer + gyroscope + magnetometer
 *              4-AR/VR Stabilized
 *              5-Gyro              up to 1kHz
 *              6-Gyro Prediction
 *          Descriptor: User Description: Orientation Estimation Method
 *          Descriptor: Characteristic Presentation Format 
 *          
 *      Characteristic: Operation
 *          Value: boolean 1-run 2-stop
 *          Descriptor: User Description: Operation Status
 *          Descriptor: Characteristic Presentation Format 
 * 
 */


// IMU Service
enum imu_gatt_chrc_t {
    CHRC_IMU_ACC,
    CHRC_IMU_ACAC,
    CHRC_IMU_ORI,
    CHRC_IMU_ORME,
    CHRC_IMU_OPR,

    CHRC_IMU_COUNT
};

enum imu_gatt_descr_t {
    DESCRIPTOR_IMU_ACC_USER,
    DESCRIPTOR_IMU_ACC_PRES,
    DESCRIPTOR_IMU_ACC_CCCD,
    DESCRIPTOR_IMU_ACAC_USER,
    DESCRIPTOR_IMU_ORI_USER,
    DESCRIPTOR_IMU_ORI_PRES,
    DESCRIPTOR_IMU_ORI_CCCD,
    DESCRIPTOR_IMU_ORME_USER,
    DESCRIPTOR_IMU_ORME_CUST,
    DESCRIPTOR_IMU_OPR_USER,
    DESCRIPTOR_IMU_OPR_PRES,

    DESCRIPTOR_IMU_COUNT
};


/* IMU Data */
extern float imu_acc[3];
extern uint8_t imu_acc_acc;
extern float imu_ori[3];
extern bool operating;

enum imu_ori_method_t {
    ORI_METHOD_GEOM,
    ORI_METHOD_GAME,
    ORI_METHOD_StGa,
    ORI_METHOD_ALLS,
    ORI_METHOD_Stab,
    ORI_METHOD_GRYO,
    ORI_METHOD_GRPR,

    ORI_METHOD_COUNT
};

// struct gatts_service_inst gatts_service_imu;
// struct gatts_char_inst gatts_char_imu[CHRC_IMU_COUNT];
// struct gatts_descr_inst gatts_descr_imu[DESCRIPTOR_IMU_COUNT]; 

/* Data R/W Handler */
void ble_update_acc(float* data);
void ble_update_ori(float* data);
void ble_update_acc_acc(uint8_t data);

#endif // _IMU_BLES_H_