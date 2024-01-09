// We want to use only core 1 for this example exercise 
// CONFIG_FREERTOS_UNICORE is defined in sdkconfig.h

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else 
static const BaseType_t app_cpu = 1;
#endif

// pin to be used 
static const int led_pin = LED_BUILTIN;

// task: blink a (built in) led
void toggleLed(void *param) {
  while(1) {
    digitalWrite(led_pin, HIGH);

    // FreeRTOS default defines the tick period to 1ms (portTICK_PERIOD_MS == 1)
    // vTaskDelay expects number of ticks, not ms. Divide desired number of ticks 
    // by tick period (i.e. we wat our delay to be 500ms)
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS); 
  }
}

void setup() { 
  pinMode(led_pin, OUTPUT);
  xTaskCreatePinnedToCore(
    toggleLed,
    "Toggle LED",   // task name 
    1024,           // stack size for this task, minimum stack size is defined in 
                    // FreeRTOSConfig.h (min stack size is size needed for empty task
                    // + overhead that the task scheduler needs)
    NULL,           // parameter to task
    1,              // task priority -> higher #, higher priority
    NULL,           // task handle
    app_cpu);         // run task on one core (ESP32)
  
  // if this was vanilla FreeRTOS, we'd have to cakll vTaskStartScheduler() in main
  // after setup is complete. On ESP32 this is called for us 
}

void loop() {

}
