#include <stdio.h>
#include <stdlib.h>

#include "driver/gpio.h"
#include "esp_ipc.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#define PRO_CPU 0
#define APP_CPU 1
#define LED_PIN 2

static const char *TAG = "Inter-Processor Call - ipc_call";
static volatile bool led_permission_flag;
static volatile uint8_t counter = 0;
TaskHandle_t Task1;
TaskHandle_t Task2;

void hello_core(void *pvParameter) {
  while (1) {
    ESP_LOGI(TAG, "Assalamualaikum World from CPU %d! at %s", xPortGetCoreID(),
             esp_log_system_timestamp());
    vTaskDelay(500 / portTICK_RATE_MS);
  }
}

void hello_ipc(void *pvParameter) {
  while (1) {
    ESP_LOGI(TAG, "Assalamualaikum World from CPU %d! at %s", xPortGetCoreID(),
             esp_log_system_timestamp());
    vTaskDelay(500 / portTICK_RATE_MS);
    counter += 1;
    if (counter == 30) {
      xSemaphoreGive(*(SemaphoreHandle_t *)pvParameter);
      break;
    }
  }
}

void led_blink(void *pvParams) {
  gpio_pad_select_gpio(LED_PIN);
  gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
  static uint8_t s_led_state = 0;
  while (1) {
    if (led_permission_flag) {
      s_led_state = !s_led_state;
      gpio_set_level(LED_PIN, s_led_state);
      ESP_LOGI(TAG, "Turning the LED %s from CPU %d! at %s",
               s_led_state == true ? "ON" : "OFF", xPortGetCoreID(),
               esp_log_system_timestamp());
    }
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}

void app_main() {
  /*
     xTaskCreatePinnedToCore
     =======================
     task function instance,
     task name,
     stack size,
     parameter to pass,
     task priority,
     task handle,
     core
  */
  UBaseType_t priority = 18;
  vTaskPrioritySet(NULL, priority);

  SemaphoreHandle_t semaphore_ipc = xSemaphoreCreateBinary();

  led_permission_flag = false;

  xTaskCreatePinnedToCore(hello_core, "hello_core", 2048, NULL, priority + 2,
                          Task1, PRO_CPU);
  vTaskDelay(500 / portTICK_RATE_MS);
  xTaskCreatePinnedToCore(led_blink, "led_blink", 2048, NULL, priority + 2,
                          Task2, APP_CPU);
  vTaskDelay(500 / portTICK_RATE_MS);

  esp_ipc_call(APP_CPU, hello_ipc, &semaphore_ipc);
  xSemaphoreTake(semaphore_ipc, 30000 / portTICK_RATE_MS);
  led_permission_flag = true;
  xSemaphoreTake(semaphore_ipc, portMAX_DELAY);
  vSemaphoreDelete(semaphore_ipc);
  vTaskPrioritySet(NULL, 5);
}
