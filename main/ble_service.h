#ifndef _BLE_SERVICE_H_
#define _BLE_SERVICE_H_

#define GATTS_CHAR_VAL_LEN_MAX  22  // TODO: Check
#include <stdint.h>
#include <esp_gatt_defs.h>
#include <esp_bt_defs.h>

/* Profile Elements */
struct gatts_service_inst {
	uint16_t gatts_if;
	uint16_t app_id;
	uint16_t conn_id;
	uint16_t service_handle;
	esp_gatt_srvc_id_t service_id;
	uint16_t num_handles;

	uint16_t chrc_count;
	uint16_t desp_count;
	struct gatts_descr_inst *descr_list;
	struct gatts_char_inst *char_list;
};

typedef esp_gatt_status_t (*attr_write_cb_t)(uint16_t, uint8_t*, uint16_t);

struct gatts_char_inst {
	uint32_t service_pos;
	esp_bt_uuid_t char_uuid;
	esp_gatt_perm_t char_perm;
	esp_gatt_char_prop_t char_property;
	esp_attr_value_t *char_val;
	esp_attr_control_t *char_control;
	uint16_t char_handle;
	// char char_nvs[16];

	attr_write_cb_t val_write;
};

struct gatts_descr_inst {
	uint32_t char_pos;
	esp_bt_uuid_t descr_uuid;
	esp_gatt_perm_t descr_perm;
	esp_attr_value_t *descr_val;
	esp_attr_control_t *descr_control;
	uint16_t descr_handle;

	attr_write_cb_t val_write;
};


#endif // _BLE_SERVICE_H_