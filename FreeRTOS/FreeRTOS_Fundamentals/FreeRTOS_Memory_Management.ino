/*
 * FreeRTOS ESP32 Project
 * -------------------------
 * Project Description:
 *   Simple program demonstrating memory management.
 *
 * Author:
 *   Morgan Lloyd
 *
 * Date:
 *   January 14, 2024
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

// change to 1024 to see stack overflow 
#define TASK_STACK_SIZE 1500

// A mundane task 
void testTask(void *param) {
  while(1){
    int a = 1;
    int b[100];

    // do something not optimized by the compiler
    for (int i = 0; i < 100; i++) {
      b[i] = a + 1;
    }
    Serial.println(b[0]);

    // Print out remaining stack memory (in words, aka "A word is a fixed-sized piece of
    // data handled as a unit by the instruction set or the hardware of the processor")
    Serial.print("High water mark (words): ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));

    // Print out number of free heap memory bytes before malloc
    Serial.print("Heap size before malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    int *ptr = (int*) pvPortMalloc(1024 * sizeof(int));
    if (ptr == NULL){
      // we are out of heap memory
      Serial.println("Not enough heap memory");
    } else {
      // modify our block of memory so compiler doesnt optimize 
      for (int i = 0; i < 100; i++) {
        ptr[i] = 3;
      }
    }
    
    // Print out number of free heap memory bytes after malloc
    Serial.print("Heap size after malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    // say no to memory leaks!
    // Comment out this line to see how the heap size changes with each loop iteration
    vPortFree(ptr);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("*****FreeRTOS Memory Management Demo*****");

  xTaskCreatePinnedToCore(testTask,
                          "Test Task",
                          TASK_STACK_SIZE,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

}

void loop() {

}