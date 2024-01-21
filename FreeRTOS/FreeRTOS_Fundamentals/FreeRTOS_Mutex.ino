/*
 * FreeRTOS ESP32 Project
 * -------------------------
 * Project Description:
 *   Program to demonstrate usage of a mutex. The mutex is used to ensure a variable that is 
 *   local to setup() is read and stored by a task (launched in setup()) before the setup() goes out
 *   of scope.
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

static const int led_pin = LED_BUILTIN;
static SemaphoreHandle_t mutex;

void task(void *param) {
  xSemaphoreTake(mutex, portMAX_DELAY);
  // copy parameter into local variable -> without mutex it would likely already
  // be out of scope 
  int delay = *(int *) param;

  // give the mutex back to setup()
  xSemaphoreGive(mutex);

  Serial.print("Received delay value: ");
  Serial.print(delay);
  pinMode(led_pin, OUTPUT);

  while (1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(delay / portTICK_PERIOD_MS);   
  }

}

void setup() {
  long int delay_arg;
  Serial.begin(115200); 

  // delaying setup task for 1s so we do not miss serial output
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("*****FreeRTOS Mutex Demo*****");
  Serial.println("Enter a delay (in ms)");

  while (Serial.available() <= 0);
  delay_arg = Serial.parseInt();
  Serial.print("Using ");
  Serial.print(delay_arg);
  Serial.println(" as a parameter to our task");

  mutex = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(task,
                        "example task",
                        1024,
                        (void*)&delay_arg, // delay arg is in local stack memory
                        1,
                        NULL,
                        app_cpu);
  
  xSemaphoreTake(mutex, portMAX_DELAY);
  Serial.println("setup() is done!");
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
