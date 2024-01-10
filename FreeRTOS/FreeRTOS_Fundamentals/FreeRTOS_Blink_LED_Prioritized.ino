/*
 * FreeRTOS ESP32 Project
 * -------------------------
 * Project Description:
 *   Simple projet demonstrating task prioritization. Lower priority tasks blinks led at random intervals 
 *   and higher priority task blinks every one second.
 *
 * Author:
 *   Morgan Lloyd
 *
 * Date:
 *   January 09, 2024
 *
 * Board:
 *   Adafruit HUZZAH32 – ESP32 Feather Board
 *
 * References:
 *   These projects were created while following along with Shawn Hymel's 
 *   YouTube series on FreeRTOS (https://www.youtube.com/playlist?list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz)
 *
 * GitHub Repository:
 *   https://github.com/morgoob/Embedded/tree/main/FreeRTOS/FreeRTOS_Fundamentals
 *
 * -------------------------
 * This code is provided as-is, without warranty or support. Use at your own risk.
 */

// We want to use only core 1 for this example exercise 
// CONFIG_FREERTOS_UNICORE is defined in sdkconfig.h

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else 
static const BaseType_t app_cpu = 1;
#endif

// pin to be used 
static const int led_pin = LED_BUILTIN;

// task: blink a (built in) led at one second intervals
void toggleLedSec(void *param) {
  while(1) {
    digitalWrite(led_pin, HIGH);

    // FreeRTOS default defines the tick period to 1ms (portTICK_PERIOD_MS == 1)
    // vTaskDelay expects number of ticks, not ms. Divide desired number of ticks 
    // by tick period (i.e. we wat our delay to be 500ms)
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS); 
  }
}

// task: blink a (built in) led at random second intervals 
void toggleLedRandom(void *param) {
  while(1) {
    int randomInterval = random(1001); // generating random number between 0 -> 1000
    digitalWrite(led_pin, HIGH);

    // FreeRTOS default defines the tick period to 1ms (portTICK_PERIOD_MS == 1)
    // vTaskDelay expects number of ticks, not ms. Divide desired number of ticks 
    // by tick period (i.e. we wat our delay to be 500ms)
    vTaskDelay(randomInterval / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(randomInterval / portTICK_PERIOD_MS); 
  }
}

void setup() { 
  randomSeed(analogRead(0)); // for random interval generation
  pinMode(led_pin, OUTPUT);
  xTaskCreatePinnedToCore(
    toggleLedSec,
    "Toggle LED at One Second Intervals",   // task name 
    1024,           // stack size for this task, minimum stack size is defined in 
                    // FreeRTOSConfig.h (min stack size is size needed for empty task
                    // + overhead that the task scheduler needs)
    NULL,           // parameter to task
    2,              // task priority -> higher #, higher priority
    NULL,           // task handle
    app_cpu);       // run task on one core (ESP32)

  xTaskCreatePinnedToCore(
    toggleLedRandom,
    "Toggle LED at Random Intervals",   // task name 
    1024,           // stack size for this task, minimum stack size is defined in 
                    // FreeRTOSConfig.h (min stack size is size needed for empty task
                    // + overhead that the task scheduler needs)
    NULL,           // parameter to task
    1,              // task priority -> higher #, higher priority
    NULL,           // task handle
    app_cpu);       // run task on one core (ESP32)

}

void loop() {

}
