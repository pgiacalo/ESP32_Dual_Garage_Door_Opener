# Garage Door Opener Project Summary

## Overview

This project creates an ESP32-based garage door opener using Espressif RainMaker that can control two separate garage doors remotely. The implementation is based on the ESP RainMaker switch example with minimal modifications to follow the established patterns.

## Key Features

### Hardware Control
- **Dual Door Support**: Controls two garage doors independently
- **Pulse Operation**: Each activation sends a 0.5-second LOW pulse to close the relay
- **Normally Open Relays**: GPIO pins are HIGH by default, go LOW only during activation
- **Safe Operation**: Relays return to open state after each pulse

### Software Features
- **RainMaker Integration**: Remote control via ESP RainMaker mobile app
- **Local Testing**: Boot button can toggle Door 1 for local testing
- **LED Indicator**: RGB LED shows when either door is active
- **Independent Control**: Each door operates independently

## Implementation Details

### GPIO Configuration
- **Door 1**: GPIO 19 (configurable in `app_priv.h`)
- **Door 2**: GPIO 21 (configurable in `app_priv.h`)
- **Boot Button**: GPIO 0 (for local testing)

### Pulse Timing
- **Duration**: 500ms (configurable via `RELAY_PULSE_DURATION_MS`)
- **Behavior**: GPIO goes LOW for duration, then returns to HIGH
- **Task-based**: Uses FreeRTOS tasks for non-blocking operation

### RainMaker Devices
- **Device Type**: Two separate switch devices
- **Names**: "Garage Door 1" and "Garage Door 2"
- **Node Name**: "Garage Door Opener"
- **Control**: Each device has independent power parameter

## File Structure

```
garage_door_opener/
├── main/
│   ├── app_main.c          # Main application with RainMaker setup
│   ├── app_driver.c        # GPIO control and pulse logic
│   ├── app_priv.h          # GPIO definitions and constants
│   └── Kconfig.projbuild   # Build configuration
├── README.md               # Project documentation
├── WIRING.md              # Hardware wiring instructions
├── test_instructions.md   # Testing procedures
├── build.sh               # Build script
└── PROJECT_SUMMARY.md     # This file
```

## Key Modifications from Switch Example

1. **Dual Device Support**: Created two separate RainMaker devices instead of one
2. **Pulse Logic**: Added FreeRTOS tasks for timed pulse generation
3. **GPIO Management**: Configured two GPIO pins with independent control
4. **State Management**: Separate state tracking for each door
5. **Callback Handling**: Modified write callback to handle both devices

## Safety Considerations

- **Isolation**: Relays provide electrical isolation between ESP32 and garage door system
- **Fail-Safe**: GPIO pins default to HIGH (relay open) state
- **Pulse Limiting**: Fixed duration prevents continuous activation
- **Independent Operation**: Each door operates independently

## Usage Instructions

1. **Hardware Setup**: Connect relays to GPIO 19 and 21
2. **Build**: Run `./build.sh` or `idf.py build`
3. **Flash**: Run `idf.py flash monitor`
4. **Configure**: Use ESP RainMaker app to provision device
5. **Control**: Use app to control doors remotely

## Customization

### GPIO Pins
Modify in `main/app_priv.h`:
```c
#define GARAGE_DOOR_1_GPIO    19
#define GARAGE_DOOR_2_GPIO    21
```

### Pulse Duration
Modify in `main/app_priv.h`:
```c
#define RELAY_PULSE_DURATION_MS  500
```

### Device Names
Modify in `main/app_main.c`:
```c
door1_device = esp_rmaker_device_create("Garage Door 1", ESP_RMAKER_DEVICE_SWITCH, NULL);
door2_device = esp_rmaker_device_create("Garage Door 2", ESP_RMAKER_DEVICE_SWITCH, NULL);
```

## Testing

- **Hardware Test**: Use multimeter to verify relay operation
- **Software Test**: Monitor serial output for pulse messages
- **App Test**: Verify RainMaker app control functionality
- **Integration Test**: Test with actual garage door controllers

This implementation provides a robust, safe, and user-friendly solution for remote garage door control using ESP32 and RainMaker technology.
