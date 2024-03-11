#include "imu_bles.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "esp_system.h"
// #include "esp_log.h"

#include "esp_bt_main.h"
#include "esp_bit_defs.h"
#include "ble_server.h"


/* Values */
static uint8_t imu_acc_bytes[12];
const uint8_t imu_acc_desc_user[20] = "Linear Acceleration";
// Format, Exponent, Unit, Namespace, Description
const uint8_t imu_acc_desc_pres[7] = {
    0x14, 0x00, 0x13, 0x27, 0x01, 0x00, 0x00
};
uint8_t imu_acc_cccd[2] = {0x00, 0x00};
bool imu_acc_notify = false;
esp_gatt_status_t imu_acc_cccd_write(uint16_t len, uint8_t* data, uint16_t offset){
    if(BIT0 & (*data)){
        // enable notification
        imu_acc_notify = true;
    }else if(BIT1 & (*data)){
        // enable indication
        imu_acc_notify = true;
    }
    imu_acc_notify = false;
    return ESP_GATT_OK;
};

static uint8_t imu_acc_acc_bytes[1] = {0};
const uint8_t imu_acc_acc_desc_user[22] = "Acceleration Accuracy";

static uint8_t imu_ori_bytes[12];
const uint8_t imu_ori_desc_user[12] = "Orientation";
const uint8_t imu_ori_desc_pres[7] = {
    0x14, 0x00, 0x63, 0x27, 0x01, 0x00, 0x00
};
uint8_t imu_ori_cccd[2] = {0x00, 0x00};
bool imu_ori_notify = false;
esp_gatt_status_t imu_ori_cccd_write(uint16_t len, uint8_t* data, uint16_t offset){
    if(BIT0 & (*data)){
        // enable notification
        imu_ori_notify = true;
    }else if(BIT1 & (*data)){
        // enable indication
        imu_ori_notify = true;
    }
    imu_ori_notify = false;
    return ESP_GATT_OK;
};

static uint8_t imu_ori_method_bytes[1];
const uint8_t imu_ori_methods[ORI_METHOD_COUNT][16] = {
    [ORI_METHOD_GEOM] = "Geomagnetic",
    [ORI_METHOD_GAME] = "Game",
    [ORI_METHOD_StGa] = "Stabilized Game",
    [ORI_METHOD_ALLS] = "Mag+Acc+Gyr",
    [ORI_METHOD_Stab] = "Stabilized",
    [ORI_METHOD_GRYO] = "Gyro",
    [ORI_METHOD_GRPR] = "Gyro Prediction",
};
uint8_t imu_ori_method_desc_cus[16];
const uint8_t imu_ori_met_desc_user[19] = "Orientation Method";
esp_gatt_status_t imu_ori_met_write(uint16_t len, uint8_t *data, uint16_t offset){
    if(*data < ORI_METHOD_COUNT){
        imu_ori_method_bytes[0] = *data;
        memcpy(imu_ori_method_desc_cus, imu_ori_methods[*data], 16);
        return ESP_GATT_OK;
    } 
    return ESP_GATT_OUT_OF_RANGE;
};

uint8_t imu_opr_val_bytes[1] = {0};
esp_gatt_status_t imu_opr_val_write(uint16_t len, uint8_t* data, uint16_t offset){
    operating = *data;
    return ESP_GATT_OK;
};
const uint8_t imu_opr_desc_pres[7] = {
    0x01, 0x00, 0x00, 0x27, 0x01, 0x00, 0x00
};
const uint8_t imu_opr_desc_user[17] = "Operation Status";

/* Service, Characteristic and Attribute Declaration */
/* Attrbutes */
// Chrc Acceleration
esp_attr_value_t serv_imu_chrc_acc_val = {
    .attr_max_len = 12,
    .attr_len = 12,
    .attr_value = imu_acc_bytes
};
esp_attr_value_t serv_imu_chrc_acc_desp_user = {
    .attr_max_len = 20,
    .attr_len = 20,
    .attr_value = imu_acc_desc_user
};
esp_attr_value_t serv_imu_chrc_acc_desp_pres = {
    .attr_max_len = 7,
    .attr_len = 7,
    .attr_value = imu_acc_desc_pres
};
esp_attr_value_t serv_imu_chrc_acc_cccd = {
    .attr_max_len = 2,
    .attr_len = 2,
    .attr_value = imu_acc_cccd
};
// Chrc Acceleration Accuracy
esp_attr_value_t serv_imu_chrc_acc_acc_val = {
    .attr_max_len = 1,
    .attr_len = 1,
    .attr_value = imu_acc_acc_bytes
};
esp_attr_value_t serv_imu_chrc_acc_acc_desp_user = {
    .attr_max_len = 22,
    .attr_len = 22,
    .attr_value = imu_acc_acc_desc_user
};

// Chrc Orientation
esp_attr_value_t serv_imu_chrc_ori_val = {
    .attr_max_len = 12,
    .attr_len = 12,
    .attr_value = imu_acc_bytes
};
esp_attr_value_t serv_imu_chrc_ori_desp_user = {
    .attr_max_len = 12,
    .attr_len = 12,
    .attr_value = imu_ori_desc_user
};
esp_attr_value_t serv_imu_chrc_ori_desp_pres = {
    .attr_max_len = 7,
    .attr_len = 7,
    .attr_value = imu_ori_desc_pres
};
esp_attr_value_t serv_imu_chrc_ori_cccd = {
    .attr_max_len = 2,
    .attr_len = 2,
    .attr_value = imu_ori_cccd
};

// Chrc Orientation Method
esp_attr_value_t serv_imu_chrc_ori_met_val = {
    .attr_max_len = 1,
    .attr_len = 1,
    .attr_value = imu_ori_method_bytes
};
esp_attr_value_t serv_imu_chrc_ori_met_desp_user = {
    .attr_max_len = 19,
    .attr_len = 19,
    .attr_value = imu_ori_met_desc_user
};
esp_attr_value_t serv_imu_chrc_ori_met_desp_cus = {
    .attr_max_len = 16,
    .attr_len = 16,
    .attr_value = imu_ori_method_desc_cus
};

// Chrc Operation Status
esp_attr_value_t serv_imu_chrc_opr_val = {
    .attr_max_len = 1,
    .attr_len = 1,
    .attr_value = imu_opr_val_bytes
};
esp_attr_value_t serv_imu_chrc_opr_desp_user = {
    .attr_max_len = 17,
    .attr_len = 17,
    .attr_value = imu_opr_desc_user
};
esp_attr_value_t serv_imu_chrc_opr_desp_pres = {
    .attr_max_len = 7,
    .attr_len = 7,
    .attr_value = imu_opr_desc_pres
};

/* Characteristics */
struct gatts_char_inst gatts_char_imu[CHRC_IMU_COUNT] = {
    [CHRC_IMU_ACC] = {
        .service_pos = 0,   // To be assigned
        .char_uuid.len = ESP_UUID_LEN_16,
        .char_uuid.uuid.uuid16 = UUID_CHRC_ACC,
        .char_perm = ESP_GATT_PERM_READ,
        .char_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_NOTIFY,
        .char_val = &serv_imu_chrc_acc_val,
        .char_control = NULL,
        .char_handle = 0    // To be assigned
    },
    [CHRC_IMU_ACAC] = {
        .service_pos = 0,   // To be assigned
        .char_uuid.len = ESP_UUID_LEN_16,
        .char_uuid.uuid.uuid16 = UUID_CHRC_ACAC,
        .char_perm = ESP_GATT_PERM_READ,
        .char_property = ESP_GATT_CHAR_PROP_BIT_READ,
        .char_val = &serv_imu_chrc_acc_acc_val,
        .char_control = NULL,
        .char_handle = 0    // To be assigned
    },
    [CHRC_IMU_ORI] = {        
        .service_pos = 0,   // To be assigned
        .char_uuid.len = ESP_UUID_LEN_16,
        .char_uuid.uuid.uuid16 = UUID_CHRC_ORI,
        .char_perm = ESP_GATT_PERM_READ,
        .char_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_NOTIFY,
        .char_val = &serv_imu_chrc_ori_val,
        .char_control = NULL,
        .char_handle = 0    // To be assigned
    },
    [CHRC_IMU_ORME] = {
        .service_pos = 0,   // To be assigned
        .char_uuid.len = ESP_UUID_LEN_16,
        .char_uuid.uuid.uuid16 = UUID_CHRC_ORME,
        .char_perm = ESP_GATT_PERM_READ,
        .char_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_PERM_WRITE,
        .char_val = &serv_imu_chrc_ori_met_val,
        .char_control = NULL,
        .char_handle = 0    // To be assigned
    },
    [CHRC_IMU_OPR] = {        
        .service_pos = 0,   // To be assigned
        .char_uuid.len = ESP_UUID_LEN_16,
        .char_uuid.uuid.uuid16 = UUID_CHRC_OPR,
        .char_perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
        .char_property = ESP_GATT_CHAR_PROP_BIT_READ,
        .char_val = &serv_imu_chrc_opr_val,
        .char_control = NULL,
        .char_handle = 0,    // To be assigned
        .val_write = imu_opr_val_write
    }
};

/* Descriptors */
struct gatts_descr_inst gatts_descr_imu[DESCRIPTOR_IMU_COUNT] = {
    [DESCRIPTOR_IMU_ACC_USER] = {
        .char_pos = CHRC_IMU_ACC,
        .descr_uuid.len = ESP_UUID_LEN_16,
        .descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_DESCRIPTION,
        .descr_perm = ESP_GATT_PERM_READ,
        .descr_val = &serv_imu_chrc_acc_desp_user,
        .descr_control = NULL,
        .descr_handle = 0       // To be assigned
    },
    [DESCRIPTOR_IMU_ACC_PRES] = {
        .char_pos = CHRC_IMU_ACC,
        .descr_uuid.len = ESP_UUID_LEN_16,
        .descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_PRESENT_FORMAT,
        .descr_perm = ESP_GATT_PERM_READ,
        .descr_val = &serv_imu_chrc_acc_desp_pres,
        .descr_control = NULL,
        .descr_handle = 0       // To be assigned
    },
    [DESCRIPTOR_IMU_ACC_CCCD] = {
        .char_pos = CHRC_IMU_ACC,
        .descr_uuid.len = ESP_UUID_LEN_16,
        .descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG,
        .descr_perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
        .descr_val = &serv_imu_chrc_acc_cccd,
        .descr_control = NULL,
        .descr_handle = 0,       // To be assigned
        .val_write = &imu_acc_cccd_write
    },
    [DESCRIPTOR_IMU_ACAC_USER] = {
        .char_pos = CHRC_IMU_ACAC,
        .descr_uuid.len = ESP_UUID_LEN_16,
        .descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_DESCRIPTION,
        .descr_perm = ESP_GATT_PERM_READ,
        .descr_val = &serv_imu_chrc_acc_acc_desp_user,
        .descr_control = NULL,
        .descr_handle = 0       // To be assigned
    },
    [DESCRIPTOR_IMU_ORI_USER] = {
        .char_pos = CHRC_IMU_ORI,
        .descr_uuid.len = ESP_UUID_LEN_16,
        .descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_DESCRIPTION,
        .descr_perm = ESP_GATT_PERM_READ,
        .descr_val = &serv_imu_chrc_ori_desp_user,
        .descr_control = NULL,
        .descr_handle = 0       // To be assigned
    },
    [DESCRIPTOR_IMU_ORI_PRES] = {
        .char_pos = CHRC_IMU_ORI,
        .descr_uuid.len = ESP_UUID_LEN_16,
        .descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_PRESENT_FORMAT,
        .descr_perm = ESP_GATT_PERM_READ,
        .descr_val = &serv_imu_chrc_ori_desp_pres,
        .descr_control = NULL,
        .descr_handle = 0       // To be assigned
    },
    [DESCRIPTOR_IMU_ORI_CCCD] = {
        .char_pos = CHRC_IMU_ORI,
        .descr_uuid.len = ESP_UUID_LEN_16,
        .descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG,
        .descr_perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
        .descr_val = &serv_imu_chrc_ori_cccd,
        .descr_control = NULL,
        .descr_handle = 0,       // To be assigned
        .val_write = &imu_ori_cccd_write
    },
    [DESCRIPTOR_IMU_ORME_USER] = {
        .char_pos = CHRC_IMU_ORME,
        .descr_uuid.len = ESP_UUID_LEN_16,
        .descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_DESCRIPTION,
        .descr_perm = ESP_GATT_PERM_READ,
        .descr_val = &serv_imu_chrc_ori_met_desp_user,
        .descr_control = NULL,
        .descr_handle = 0       // To be assigned
    },
    [DESCRIPTOR_IMU_ORME_CUST] = {
        .char_pos = CHRC_IMU_ORME,
        .descr_uuid.len = ESP_UUID_LEN_16,
        .descr_uuid.uuid.uuid16 = UUID_DESP_OPME_STR,
        .descr_perm = ESP_GATT_PERM_READ,
        .descr_val = &serv_imu_chrc_ori_met_desp_user,
        .descr_control = NULL,
        .descr_handle = 0,       // To be assigned
        .val_write = &imu_opr_val_write
    },
    [DESCRIPTOR_IMU_OPR_USER] = {
        .char_pos = CHRC_IMU_OPR,
        .descr_uuid.len = ESP_UUID_LEN_16,
        .descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_DESCRIPTION,
        .descr_perm = ESP_GATT_PERM_READ,
        .descr_val = &serv_imu_chrc_opr_desp_user,
        .descr_control = NULL,
        .descr_handle = 0       // To be assigned
    },
    [DESCRIPTOR_IMU_OPR_PRES] = {
        .char_pos = CHRC_IMU_OPR,
        .descr_uuid = {
            .len = ESP_UUID_LEN_16,
            .uuid.uuid16 = ESP_GATT_UUID_CHAR_PRESENT_FORMAT,
        },
        .descr_perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
        .descr_val = &serv_imu_chrc_opr_desp_pres,
        .descr_control = NULL,
        .descr_handle = 0       // To be assigned
    }
};

/* Services */
struct gatts_service_inst gatts_service_imu = {
    .gatts_if = ESP_GATT_IF_NONE,   // To be assigned
    .num_handles = DESCRIPTOR_IMU_COUNT + CHRC_IMU_COUNT + 1,

    .chrc_count = CHRC_IMU_COUNT,
    .desp_count = DESCRIPTOR_IMU_COUNT,
    .descr_list = gatts_descr_imu,
    .char_list = gatts_char_imu
};


void ble_update_acc(float* data){
    static uint8_t float_buffer[4];

    *(float*)(float_buffer) = data[0];
    imu_acc_bytes[0] = float_buffer[0];
    imu_acc_bytes[1] = float_buffer[1];
    imu_acc_bytes[2] = float_buffer[2];
    imu_acc_bytes[3] = float_buffer[3];
    *(float*)(float_buffer) = data[1];
    imu_acc_bytes[4] = float_buffer[0];
    imu_acc_bytes[5] = float_buffer[1];
    imu_acc_bytes[6] = float_buffer[2];
    imu_acc_bytes[7] = float_buffer[3];
    *(float*)(float_buffer) = data[2];
    imu_acc_bytes[8] = float_buffer[0];
    imu_acc_bytes[9] = float_buffer[1];
    imu_acc_bytes[10] = float_buffer[2];
    imu_acc_bytes[11] = float_buffer[3];

    if(imu_acc_notify){
        gatt_notify(gatts_char_imu[CHRC_IMU_ACC].char_handle, 
                    sizeof(imu_acc_bytes), imu_acc_bytes, false);
    }
}

void ble_update_acc_acc(uint8_t data){
    imu_acc_acc_bytes[0] = data;
}

void ble_update_ori(float* data){
    static uint8_t float_buffer[4];

    *(float*)(float_buffer) = data[0];
    imu_ori_bytes[0] = float_buffer[0];
    imu_ori_bytes[1] = float_buffer[1];
    imu_ori_bytes[2] = float_buffer[2];
    imu_ori_bytes[3] = float_buffer[3];
    *(float*)(float_buffer) = data[1];
    imu_ori_bytes[4] = float_buffer[0];
    imu_ori_bytes[5] = float_buffer[1];
    imu_ori_bytes[6] = float_buffer[2];
    imu_ori_bytes[7] = float_buffer[3];
    *(float*)(float_buffer) = data[2];
    imu_ori_bytes[8] = float_buffer[0];
    imu_ori_bytes[9] = float_buffer[1];
    imu_ori_bytes[10] = float_buffer[2];
    imu_ori_bytes[11] = float_buffer[3];

    if(imu_ori_notify){
        gatt_notify(gatts_char_imu[CHRC_IMU_ORI].char_handle, 
                    sizeof(imu_acc_bytes), imu_acc_bytes, false);
    }
}