/*
 * FreeRTOS ESP32 Project
 * -------------------------
 * Project Description:
 *   Program to demonstrate use of a task-safe queue. 
 *   TaskA: Prints messages from queue2, reads and echos serial input, if serial input is
 *          "delay xxx" it sends xxx (int) to queue1
 *   TaskB: Reads values from queue1 to update a local variable that controls the blink
 *          rate of an LED. Everytime the LED blinks 100 times it sends "Blinked" to queue2 
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
static const int LED_PIN = LED_BUILTIN;
static const uint8_t QUEUE_LEN = 16;
// max serial IO line size 
static const int BUFFER_SIZE = 256;

// queue1 stores integers that will control the blink rate of an LED
static QueueHandle_t queue1;
// queue2 stores strings to be printed to the serial terminal 
static QueueHandle_t queue2;

// TaskA: Prints messages from queue2, reads and echos serial input, if serial input is
//        "delay xxx" it sends xxx (int) to queue1
void taskA(void *param) {
  static char buffer[BUFFER_SIZE]; // 256 bytes allocated to read and echo serial IO 
  static int bufferIndex = 0;
  char queue2Buffer[BUFFER_SIZE];
  memset(queue2Buffer, 0, sizeof(buffer));
  int item;

  while (1) {
    if (xQueueReceive(queue2, (void*)queue2Buffer, 10) == pdTRUE) {
      Serial.println(queue2Buffer);
    }
    while (Serial.available() > 0) {
      char charRead = Serial.read();

      if (charRead == '\n') {
        buffer[bufferIndex] = '\0'; // null terminator 
        if (strncmp(buffer, "delay ", 6) == 0) {
          int delayValue;
          if (sscanf(buffer + 6, "%d", &delayValue) == 1) {
            // Successfully got delay value 
            Serial.print("Received delay value: ");
            Serial.println(delayValue);
            if (xQueueSend(queue1, (void*)&delayValue, 10) != pdTRUE) {
              Serial.println("queue1 is FULL, could not enter delay value");
            }
          } else {
            Serial.println("Invalid delay command format");
          }
        }
        Serial.println(buffer);
        bufferIndex = 0;
        memset(buffer, 0, sizeof(buffer));
      } else {
        if (bufferIndex < BUFFER_SIZE - 1) {
          buffer[bufferIndex] = charRead;
          bufferIndex++;
        } else {
          bufferIndex = 0;
          // Set all bytes in the buffer to NULL
          memset(buffer, 0, sizeof(buffer));
        }
      }
    }
  }
}

// TaskB: Reads values from queue1 to update a local variable that controls the blink
//        rate of an LED. Everytime the LED blinks 100 times it sends "Blinked" to queue2 
void taskB(void *param) {
  static int delay = 1000;
  static int blinkCount = 0; 
  const char blink_str[] = "Blinked";  
  while(1){

    // do not need to check for pdFALSE, delay will be previous value 
    xQueueReceive(queue1, (void*)&delay, 0);
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(delay / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(delay / portTICK_PERIOD_MS); 
    blinkCount++;

    if (blinkCount == 100) {
      if (xQueueSend(queue2, (void*)blink_str, 10) != pdTRUE) {
        Serial.println("queue1 is FULL, could not enter delay value");
      }
      blinkCount = 0;
    }
  }
}

void setup() {
  Serial.begin(115200); 
  pinMode(LED_PIN, OUTPUT);

  // delaying setup task for 1s so we do not miss serial output
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("*****FreeRTOS Task-Safe Queue Demo II*****");

  // Create queue
  queue1 = xQueueCreate(QUEUE_LEN, sizeof(int));
  queue2 = xQueueCreate(QUEUE_LEN, BUFFER_SIZE);

  xTaskCreatePinnedToCore(taskA,
                        "taskA",
                        2048,
                        NULL,
                        1,
                        NULL,
                        app_cpu);

  xTaskCreatePinnedToCore(taskB,
                        "taskB",
                        2048,
                        NULL,
                        1,
                        NULL,
                        app_cpu);
}

void loop() {
}
