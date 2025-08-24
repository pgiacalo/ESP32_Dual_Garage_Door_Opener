/* Garage Door Opener implementation using two relays
   
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <sdkconfig.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>

#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_params.h>
#include <esp_rmaker_standard_devices.h>

#include <app_reset.h>
#include <ws2812_led.h>
#include "app_priv.h"

// External declarations for device pointers
extern esp_rmaker_device_t *door1_device;
extern esp_rmaker_device_t *door2_device;

static const char *TAG = "app_driver";

/* These values correspond to H,S,V = 120,100,10 */
#define DEFAULT_RED     0
#define DEFAULT_GREEN   25
#define DEFAULT_BLUE    0

// Global state variables - track if a pulse is currently active
static bool g_door1_pulse_active = false;
static bool g_door2_pulse_active = false;

static void app_indicator_set(bool state)
{
    if (state) {
        ws2812_led_set_rgb(DEFAULT_RED, DEFAULT_GREEN, DEFAULT_BLUE);
    } else {
        ws2812_led_clear();
    }
}

static void app_indicator_init(void)
{
    ws2812_led_init();
    // Show indicator based on pulse states (ON if either door is pulsing)
    app_indicator_set(g_door1_pulse_active || g_door2_pulse_active);
}

// Simple pulse task for Door 1
static void door1_pulse_task(void *pvParameters)
{
    if (g_door1_pulse_active) {
        ESP_LOGW(TAG, "Door 1 pulse already active, ignoring");
        vTaskDelete(NULL);
        return;
    }
    
    ESP_LOGI(TAG, "Door 1: Starting pulse (GPIO %d LOW for %d ms)", GARAGE_DOOR_1_GPIO, RELAY_PULSE_DURATION_MS);
    
    g_door1_pulse_active = true;
    app_indicator_set(g_door1_pulse_active || g_door2_pulse_active);
    
    // Set GPIO LOW to close relay
    gpio_set_level(GARAGE_DOOR_1_GPIO, 0);
    
    // Wait for pulse duration
    vTaskDelay(pdMS_TO_TICKS(RELAY_PULSE_DURATION_MS));
    
    // Set GPIO HIGH to open relay
    gpio_set_level(GARAGE_DOOR_1_GPIO, 1);
    
    g_door1_pulse_active = false;
    app_indicator_set(g_door1_pulse_active || g_door2_pulse_active);
    
    // Report state back to RainMaker (OFF)
    esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_name(door1_device, ESP_RMAKER_DEF_POWER_NAME),
            esp_rmaker_bool(false));
    
    ESP_LOGI(TAG, "Door 1: Pulse completed, reported OFF to RainMaker");
    
    // Delete the task
    vTaskDelete(NULL);
}

// Simple pulse task for Door 2
static void door2_pulse_task(void *pvParameters)
{
    if (g_door2_pulse_active) {
        ESP_LOGW(TAG, "Door 2 pulse already active, ignoring");
        vTaskDelete(NULL);
        return;
    }
    
    ESP_LOGI(TAG, "Door 2: Starting pulse (GPIO %d LOW for %d ms)", GARAGE_DOOR_2_GPIO, RELAY_PULSE_DURATION_MS);
    
    g_door2_pulse_active = true;
    app_indicator_set(g_door1_pulse_active || g_door2_pulse_active);
    
    // Set GPIO LOW to close relay
    gpio_set_level(GARAGE_DOOR_2_GPIO, 0);
    
    // Wait for pulse duration
    vTaskDelay(pdMS_TO_TICKS(RELAY_PULSE_DURATION_MS));
    
    // Set GPIO HIGH to open relay
    gpio_set_level(GARAGE_DOOR_2_GPIO, 1);
    
    g_door2_pulse_active = false;
    app_indicator_set(g_door1_pulse_active || g_door2_pulse_active);
    
    // Report state back to RainMaker (OFF)
    esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_name(door2_device, ESP_RMAKER_DEF_POWER_NAME),
            esp_rmaker_bool(false));
    
    ESP_LOGI(TAG, "Door 2: Pulse completed, reported OFF to RainMaker");
    
    // Delete the task
    vTaskDelete(NULL);
}

void app_driver_init()
{
    // Set GPIO pins HIGH IMMEDIATELY to prevent relay activation during startup
    gpio_set_level(GARAGE_DOOR_1_GPIO, 1);
    gpio_set_level(GARAGE_DOOR_2_GPIO, 1);
    
    /* Configure GPIO pins for both garage doors */
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 1,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE,
    };
    
    // Configure Door 1 GPIO
    io_conf.pin_bit_mask = ((uint64_t)1 << GARAGE_DOOR_1_GPIO);
    gpio_config(&io_conf);
    
    // Configure Door 2 GPIO
    io_conf.pin_bit_mask = ((uint64_t)1 << GARAGE_DOOR_2_GPIO);
    gpio_config(&io_conf);
    
    // Set initial states again (HIGH = relay open) - double safety
    gpio_set_level(GARAGE_DOOR_1_GPIO, 1);
    gpio_set_level(GARAGE_DOOR_2_GPIO, 1);
    
    // Add a small delay to ensure pins are stable
    vTaskDelay(pdMS_TO_TICKS(100));
    
    app_indicator_init();
}

int IRAM_ATTR app_driver_set_door1_state(bool state)
{
    if (state) {
        // Create a task to handle the pulse (to avoid blocking the callback)
        xTaskCreate(door1_pulse_task, "door1_pulse", 2048, NULL, 5, NULL);
    }
    return ESP_OK;
}

int IRAM_ATTR app_driver_set_door2_state(bool state)
{
    if (state) {
        // Create a task to handle the pulse (to avoid blocking the callback)
        xTaskCreate(door2_pulse_task, "door2_pulse", 2048, NULL, 5, NULL);
    }
    return ESP_OK;
}

bool app_driver_get_door1_state(void)
{
    return g_door1_pulse_active;
}

bool app_driver_get_door2_state(void)
{
    return g_door2_pulse_active;
}
