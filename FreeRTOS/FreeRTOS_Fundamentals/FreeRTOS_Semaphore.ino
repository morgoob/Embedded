/*
 * FreeRTOS ESP32 Project
 * -------------------------
 * Project Description:
 *   Program to demonstrate usage of a semaphore. The semaphore is used to ensure a Message that is 
 *   local to setup() is read and stored by 5 separate tasks before setup() goes out of scope. A
 *   mutex is used to protect the print statements
 *
 * Author:
 *   Morgan Lloyd
 *
 * Date:
 *   January 22, 2024
 *
 * Board:
 *   Adafruit HUZZAH32 – ESP32 Feather Board
 *
 * References:
 *   These projects were created while following along with Shawn Hymel's 
 *   YouTube series on FreeRTOS (https://www.youtube.com/playlist?list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz)
 * 
 * API:
 *   https://www.freertos.org/a00018.html
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

// How we will pass a string as a parameter to our tasks
typedef struct Message {
  char body[23];
  uint8_t len;
} Message;

static const int num_tasks = 5;

static SemaphoreHandle_t semaphore;
static SemaphoreHandle_t mutex;

void task(void *param) {

  Message msg = *(Message*)param;

  // Increment the semaphore to show that the parameter has been read
  xSemaphoreGive(semaphore);

  // Take mutex (critical section)
  xSemaphoreTake(mutex, portMAX_DELAY);
  Serial.print("Message received: ");
  Serial.print(msg.body);
  Serial.print(" | Length: ");
  Serial.println(msg.len);

  // Leaving critical section, give mutex
  xSemaphoreGive(mutex);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL);
}

void setup() {
  char task_name[12];
  char text[23] = "Goodmorning starshine!";
  text[22] = '\0';
  Message msg;
  Serial.begin(115200); 

  // delaying setup task for 1s so we do not miss serial output
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("*****FreeRTOS Semaphore Demo*****");
  
  // Create semaphore and initialize to 0
  semaphore = xSemaphoreCreateCounting(num_tasks, 0);

  // Create mutex to protect print statements
  mutex = xSemaphoreCreateMutex();

  strcpy(msg.body, text);
  msg.len = strlen(text);

  // Launch our tasks
  for (int i = 0; i < num_tasks; i++) {
    sprintf(task_name, "Task %i", i);
    xTaskCreatePinnedToCore(task,
                        task_name,
                        1024,
                        (void*)&msg,
                        1,
                        NULL,
                        app_cpu);
  }

  // When semaphore is decremented down to 0 we know all tasks have saved the message
  for (int i = 0; i < num_tasks; i++) {
    xSemaphoreTake(semaphore, portMAX_DELAY);
  }
  
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}