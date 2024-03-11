/* Copyright 2017 Heiko Rothkranz
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BLE_SERVER_H_
#define BLE_SERVER_H_

#include "imu_bles.h"
#include <stdint.h>
#include <esp_gatt_defs.h>
#include <esp_gatts_api.h>

#define GATTS_TAG "GATT_SERVER"

/* GATT Service Parameters */
#define BLE_DEVICE_NAME_LEN 6
#define BLE_DEVICE_NAME "MyIMU"
#define BLE_MANUFACTURER_DATA_LEN	2					// Length of the manufacturer specific advertising payload. Currently contains only the manufacturer ID (2 bytes)
#define BLE_MANUFACTURER_DATA		{0xFF, 0xFF}        // No manufacturer ID
#define BLE_APPEARANCE              0x541               // Motion Sensor

/* Advertisement */
#define BLE_ADV_SERVICE_DATA_LEN    0
#define BLE_ADV_SERVICE_DATA        NULL                // TODO: can advertise ourself
#define BLE_ADV_MIN_INTERVAL        0x0020              // unit: 1.25ms
#define BLE_ADV_MAX_INTERVAL        0x0100              // unit: 1.25ms

/* Connection */
#define BLE_CONNECTED_MAX_INTERVAL	0x80		// Min connection interval (X * 1.25ms, TODO: to be confirmed) -> 500ms
#define BLE_CONNECTED_MIN_INTERVAL	0x10		// Max connection interval (X * 1.25ms, TODO: to be confirmed) -> 1000ms
#define BLE_CONNECTED_TIMEOUT		0x320	    // Supervision timeout for the LE Link. Range: 0x000A to 0x0C80 (X * 10ms) -> 4000ms

/* Profile Elements */
enum gatt_app{
    APP_ID_IMU,

    APP_ID_COUNT
};

enum gatt_service{
    SERVICE_ID_IMU,

    SERVICE_ID_COUNT
};

extern struct gatts_service_inst gatts_service_imu;
extern struct gatts_char_inst gatts_char_imu[CHRC_IMU_COUNT];
extern struct gatts_descr_inst gatts_descr_imu[DESCRIPTOR_IMU_COUNT]; 

#define GATTS_CHAR_NUM CHRC_IMU_COUNT // + CHRC_BAS_COUNT
#define GATTS_DESCR_NUM DESCRIPTOR_IMU_COUNT // + DESCRIPTOR_BAS_COUNT

struct connection_t{
    uint16_t conn_id;
    esp_gatt_if_t gatts_if;
    bool connected;
};

void gaps_init();
void gaps_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);

void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
void gatt_notify(uint16_t handle, uint16_t value_len, uint8_t* value, bool need_confirm);

#endif /* BLE_SERVER_H_ */