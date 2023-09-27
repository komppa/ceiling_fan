# Ceiling Fan

## Overview
Welcome to the Ceiling Fan project! By using this project you can control your old PC fan seamlessly using the MQTT protocol. Just wire fan and relay to your board & flash it! This project leverages an ESP8266 chip combined with a computer fan housed within a custom 3D printed enclosure.

## Prerequisites
- A computer fan
- ESP8266 chip
- 5V relay board (for turning the fan on/off)
- 12V power supply
- 3D printed enclosure (design files provided in the repository for 100mm HVAC pipe)

## Configuration
Before you begin, you need to insert following details to **wifi_credentials.ini** file:
1. Insert your WiFi credentials (`SSID` and `Password`).
2. Provide your MQTT server address.

**Note**: Currently, the MQTT port is set to 1883 and cannot be changed via the configuration file. To change the port, you need to modify the code (src/main.cpp)

## Compilation
Compile and upload the code using [PlatformIO](https://platformio.org/).

## Using on simulator

Wokwi simulator can be used to test the code without having to flash the ESP8266 chip. To use the simulator, needed files are provided in the repository (wokwi.toml & diagram.json). If you are using paid version of the simulator, you can use the network feature to test the MQTT connection. The project is intended to be used with ESP8266 chip, so the default PIO environment will be that one. If building using environment 'simulator', precompiler flags are used to check whether the build is for the simulator (meaning ESP32 board since only ESP8266 is not supported on simulator).

## Contributions
Feel free to contribute to this project, submit issues, and pull requests. Any feedback or enhancement is appreciated.

