# BLE MOTION

This is a project of a motion sensor server based on BNO080 and a ESP32 chip. It reports its data via Bluetooth Low Energy(BLE). The development environment is ESP-IDF v5.2.1.

## Build Project

1. Clone the [IMU library](https://github.com/myles-parfeniuk/esp32_BNO08x.git) to `components` folder. 2. Check the Bluetooth support in sdkconfig

## BLE Service

The GATT Server is implemented refering to ESP GATT Server example and [Heiko Rothkranz's example](https://gist.github.com/heiko-r/f284d95141871e12ca0164d9070d61b4). Attemptions are made to generalize the BLE Service definition.

To create a new customized BLE application, you can follow the steps below:

* create the backend that generates data and handles requests. For example, the `imu.h` and `imu.c` defines a IMU instance and a thread task to update the motion data. `ble_update_xxx` are going to be implemented in the BLE server.
* define the services using the ATT elements' structures defined in `ble_service.h`. For example, `imu_bles.h` enumerates the characteristics, descriptors in IMU service and `imu_bles.c` defines these elements and functions 1. to update attribute value by actual data(and generate notification); 2. callback to client write operation;
* set GAP parameters in `ble_server.h`. e.g. your device name, connection interval, ...
* in the main program, after setting up the bluetooth as shown in gatt_server example, call `gaps_init()` to start the service.

## IMU: BNO08x

[https://github.com/myles-parfeniuk/esp32_BNO08x.git](https://github.com/myles-parfeniuk/esp32_BNO08x.git) is used as the library for reading BNO08x.

To make debugging easier, I recommend you to add this code block after [this line](https://github.com/myles-parfeniuk/esp32_BNO08x/blob/05006334201b1a22d85603df19d637130d5c408d/BNO08x.cpp#L352):

```cpp
    if(packet_length_rx == 0xFFFF){
        ESP_LOGE(TAG, "Peripheral Error.");
        return false;
    }
```

so that you are less likely to start debugging software before handling the hardware errors.