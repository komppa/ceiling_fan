# Ceiling Fan

## Overview
Welcome to the Ceiling Fan project! This project leverages an ESP8266 chip combined with a computer fan housed within a custom 3D printed enclosure. Users can control the fan seamlessly using the MQTT protocol.

## Prerequisites
- A computer fan
- ESP8266 chip
- 5V relay board
- 12V power supply
- 3D printed enclosure (design files provided in the repository)

## Configuration
Before you begin, you need to insert following details to **wifi_credentials.ini** file:
1. Insert your WiFi credentials (`SSID` and `Password`).
2. Provide your MQTT server address.

**Note**: Currently, the MQTT port is set to 1883 and cannot be changed via the configuration file.

## Compilation
Compile and upload the code using [PlatformIO](https://platformio.org/).

## Contributions
Feel free to contribute to this project, submit issues, and pull requests. Any feedback or enhancement is appreciated.

