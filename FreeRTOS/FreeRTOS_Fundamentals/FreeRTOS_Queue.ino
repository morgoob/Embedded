/*
 * FreeRTOS ESP32 Project
 * -------------------------
 * Project Description:
 *   Simple program to demonstrate use of a task-safe queue. Currently loop() adds items to 
 *   the queue faster than they are read, adjust the delay times to view different queue behavior
 *
 * Author:
 *   Morgan Lloyd
 *
 * Date:
 *   January 15, 2024
 *
 * Board:
 *   Adafruit HUZZAH32 – ESP32 Feather Board
 *
 * References:
 *   These projects were created while following along with Shawn Hymel's 
 *   YouTube series on FreeRTOS (https://www.youtube.com/playlist?list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz)
 * 
 * API:
 * https://www.freertos.org/a00018.html
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

// Private (static) globals
static const uint8_t QUEUE_LEN = 5;
static QueueHandle_t queue;

// Task: wait for a queue item and print it 
void printMessage(void *param) {
  int item;

  while(1){

    // timeout == 0 ticks, this function call is non-blocking
    // returns pdTrue if something was read from the queue
    if (xQueueReceive(queue, (void*)&item, 0) == pdTRUE) {
      Serial.println(item);
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200); 

  // delaying setup task for 1s so we do not want to miss serial output
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("*****FreeRTOS Task-Safe Queue Demo*****");

  // Create queue
  queue = xQueueCreate(QUEUE_LEN, sizeof(int));

  xTaskCreatePinnedToCore(printMessage,
                        "Print Queue Items",
                        1024,
                        NULL,
                        1,
                        NULL,
                        app_cpu);
}

void loop() {
  // static num will retain value between loop iterations
  static int num = 0;

  if (xQueueSend(queue, (void*)&num, 10) != pdTRUE) {

    // For demo purposes only -> good practice to do all serial IO in one task
    Serial.println("Queue is FULL");
  }

  num++;
  vTaskDelay(500 / portTICK_PERIOD_MS);
}
