# Accelerometer and Sensor Monitor

This Arduino script is designed to monitor data from an accelerometer and various sensors while communicating the information over Bluetooth Low Energy (BLE). It provides real-time information about movement, obstacle detection, and accelerometer data.

## Hardware Requirements

- Arduino board with BLE support (e.g., Arduino Nano 33 BLE)
- ADXL343 accelerometer
- IR sensors

## Libraries Used

- Adafruit Sensor Library
- Adafruit ADXL343 Library
- DFRobot RGBLCD1602 Library
- ArduinoBLE Library

## Configuration

1. Ensure that the required libraries are installed in your Arduino IDE.
2. Connect the hardware components to the designated pins on your Arduino board.
3. Upload this script to your Arduino board.

## Functionality

- Monitors accelerometer data (X, Y, Z axes) and sends it over BLE.
- Detects obstacles using IR sensors and reports their status over BLE.
- Indicates movement detection and sends the status over BLE.
- Displays data on an RGB LCD screen.

## BLE Services

- **Service UUID:** 3096261e-325a-42ff-b1a2-0401d482e2f9
- **Characteristics:**
    - `COORD_UUID` for accelerometer data
    - `MOVEMENT_UUID` for movement status
    - `OBSTACLE_UUID` for obstacle detection
