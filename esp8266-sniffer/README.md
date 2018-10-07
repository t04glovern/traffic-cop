# ESP8266 WiFi AP Sniffer

## Setup

Simply flash this firmware to the ESP8266 board.

## Platform IO

This project is build and run with PlatformIO. The library dependencies can be found in the `platformio.ini` file. Below is the current configuration targetting the NodeMCU varient of the ESP8266 development board.

```ini
[env:nodemcuv2]
platform = espressif8266
board = nodemcuv2
framework = arduino
monitor_speed = 115200

lib_deps =
  PJON@11.1
```

## References

* [Based on Ray Burnette's ESP8266 Mini Sniff (MIT)](https://www.hackster.io/rayburne/esp8266-mini-sniff-f6b93a)
* [RandDruid/esp8266-deauth (MIT)](https://github.com/RandDruid/esp8266-deauth)
* [kripthor/WiFiBeaconJam](https://github.com/kripthor/WiFiBeaconJam)
