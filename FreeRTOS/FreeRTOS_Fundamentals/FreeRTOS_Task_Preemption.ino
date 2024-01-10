/*
 * FreeRTOS ESP32 Project
 * -------------------------
 * Project Description:
 *   Simple program to demonstrate task preemption.
 *
 * Author:
 *   Morgan Lloyd
 *
 * Date:
 *   January 10, 2024
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

// Using only core one for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else 
static const BaseType_t app_cpu = 1;
#endif

// Task Handles for task management 
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

// A random string to print 
const char *msg = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";

// Task with lower priority printing a random string 
void printMsg(void *param) {
  int msg_len = strlen(msg);

  // print msg to serial terminal one character at a time so we can obviously see task preemption
  while(1) {
    Serial.println();
    for (int i = 0; i < msg_len; i++) {
      Serial.print(msg[i]);
    }
    Serial.println();

    // put task into blocked state for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Task with higher priority to preempt task1. This task prints an obvious char '*' so we can see
// the preemption
void printStar(void *param) {
  while(1) {
    // print an '*' every 100ms 
    Serial.print('*');
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() { 
  // configuring serial terminal at very low baudrate so we can obviously see preemption
  Serial.begin(300); 

  // delaying setup task for 1s so we do not want to miss serial output
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("*****FreeRTOS Task Preemption Demo*****");

  // Print self (setup task) info
  Serial.print("setup and loop task running on core: ");
  Serial.print(xPortGetCoreID());
  Serial.print(" with priority: ");
  Serial.println(uxTaskPriorityGet(NULL));
  
  xTaskCreatePinnedToCore(
    printMsg,
    "Print Random Message",   
    1024,           // stack size for this task, minimum stack size is defined in 
                    // FreeRTOSConfig.h (min stack size is size needed for empty task
                    // + overhead that the task scheduler needs)
    NULL,           // parameter to task
    1,              // task priority -> higher #, higher priority
    &task_1,        // task handle
    app_cpu);       // run task on one core (ESP32)


  xTaskCreatePinnedToCore(
    printStar,
    "Print Star",   
    1024,           // stack size for this task, minimum stack size is defined in 
                    // FreeRTOSConfig.h (min stack size is size needed for empty task
                    // + overhead that the task scheduler needs)
    NULL,           // parameter to task
    2,              // task priority -> higher #, higher priority
    &task_2,        // task handle
    app_cpu);       // run task on one core (ESP32)
}

void loop() {

}
