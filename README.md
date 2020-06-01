# LwM2M for ESP-32

## Original repo

[github:\\\eclipse/wakaama](https://github.com/eclipse/wakaama)

## How to use

* Use `platformio` extension
* Set up wifi credentials in `platformio.ini`
* Set up name and server url in `LwM2mManager.h`
* Connect ESP32
* Upload firmware


## DTLS

* Uncomment `-DWITH_TINYDTLS` in `platformio.ini`
* Set up PSK in `main.cpp`

<!-- Just uncomment flag `-DWITH_BLE `  -->

## In plan

* Separate code
* Move settings in `platfomrio.ini`
* DTLS testing
* Add smart-object manager (for easier adding)
* App to connect to device (Bluetooth)
* Add Bluetooth
  * set up wifi credentials, change client name and server urls (saving in Preferences)
  * upload new firmware
* Extend firmware for ESP8266
* \*LwM2M via BLE 

## Size problem with BLE

Uncomment parameter `board_build.partitions = no_ota.csv`

[github:\\\ESP-32 Partitions](https://github.com/espressif/arduino-esp32/tree/master/tools/partitions)

[platformio:\\\Partition-tables](https://docs.platformio.org/en/latest/platforms/espressif32.html#partition-tables)

[github:\\\Size issue](https://github.com/espressif/arduino-esp32/issues/1075#issuecomment-459393220)

## License

Eclipse Public