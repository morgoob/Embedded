# FreeRTOS Fundamentals

This directory contains fundamental examples of Real-Time Operating Systems (RTOS) using FreeRTOS, specifically implemented on the Adafruit HUZZAH32 – ESP32 Feather Board.

## Introduction

Welcome to the FreeRTOS Fundamentals repository! This collection of projects serves as an introduction to Real-Time Operating Systems (RTOS) concepts using FreeRTOS. Each example is implemented on the Adafruit HUZZAH32 – ESP32 Feather Board.

## Projects
The following list of projects ascend by increasing complexity.

1. [**Task Creation**](FreeRTOS_Blinky.ino) 
2. [**Task Prioritization**](FreeRTOS_Blinky_Prioritized.ino) 
3. [**Task Preemption**](FreeRTOS_Task_Preemption.ino) 
4. [**Memory Management**](FreeRTOS_Memory_Management.ino)


## Getting Started

### Setting Up ESP32 on Arduino IDE

Follow these steps to set up the ESP32 on the Arduino IDE:

1. Download the FreeRTOS source code from [freertos.org](https://www.freertos.org/).
2. In Arduino IDE, select “File” → “Preferences” → URL List button → Enter: `https://dl.espressif.com/dl/package_esp32_index.json` (this file contains the board definition and configurations).
3. “Tools” → “Board” → “Boards Manager” → Search for “esp32” and select the esp32 package by Espressif Systems → Install the most recent version.
4. Once the install completes, exit and reload the Arduino IDE. Restarting the application allows it to reload its configuration, recognize the newly installed board support, and make the added features and boards accessible within the IDE.
5. If you want to view how FreeRTOS is configured on the esp32, you can view `FreeRTOSConfig.h`. If the Arduino IDE was installed from the home directory, this configuration file is likely found in:
   `/home/user/.arduino15/packages/esp32/hardware/esp32/2.0.11/tools/sdk/esp32/include/freertos/include/esp_additions/freertos/FreeRTOSConfig.h`

## Contributing

If you find any errors, have suggestions, or want to contribute, please open an issue or submit a pull request.

## Credit
These projects were created while following along with Shawn Hymel's YouTube series on FreeRTOS. Check out his [FreeRTOS Tutorials](https://www.youtube.com/playlist?list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz) for in-depth explanations and additional insights.

## Author

- [Morgan Lloyd](https://github.com/morgoob)
