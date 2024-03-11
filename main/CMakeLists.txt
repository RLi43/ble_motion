idf_component_register(SRCS "gatts_demo.c" "imu.c" "imu_bles.c" "ble_server.c"
                    INCLUDE_DIRS "." "..components/esp32_BNO08x")

add_compile_definitions("ESP32C3_IMU_CONFIG")