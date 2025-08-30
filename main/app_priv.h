/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once
#include <stdint.h>
#include <stdbool.h>

// GPIO pins for the two garage doors
// These pins are compatible with both ESP32 (0-33 output range) and ESP32C6 (0-30 output range)
#define GARAGE_DOOR_1_GPIO    15  // First garage door relay control
#define GARAGE_DOOR_2_GPIO    19  // Second garage door relay control

// Default states (HIGH = relay open, LOW = relay closed)
#define DEFAULT_DOOR_1_STATE  true   // HIGH by default
#define DEFAULT_DOOR_2_STATE  true   // HIGH by default

// Pulse duration for relay activation (0.5 seconds)
#define RELAY_PULSE_DURATION_MS  500

extern esp_rmaker_device_t *door1_device;
extern esp_rmaker_device_t *door2_device;

void app_driver_init(void);
int app_driver_set_door1_state(bool state);
int app_driver_set_door2_state(bool state);
bool app_driver_get_door1_state(void);
bool app_driver_get_door2_state(void);
