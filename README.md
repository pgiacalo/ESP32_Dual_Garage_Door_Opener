# Garage Door Opener Example

This example demonstrates how to create a dual garage door opener using ESP RainMaker. The ESP32 controls two normally open relays that are connected in parallel to wired pushbutton controllers inside the garage.

## Features

- **Dual Door Control**: Control two separate garage doors independently
- **Pulse Operation**: Each door activation sends a 0.5-second LOW pulse to close the relay
- **RainMaker Integration**: Control doors remotely via the ESP RainMaker mobile app
- **Local Button Control**: Physical button can toggle Door 1 for local testing
- **LED Indicator**: RGB LED shows when either door is active

## Hardware Setup

### GPIO Connections
- **Door 1**: GPIO 19 (default HIGH, goes LOW for 0.5s when activated)
- **Door 2**: GPIO 21 (default HIGH, goes LOW for 0.5s when activated)
- **Boot Button**: GPIO 0 (for local testing and WiFi reset)

### Relay Wiring
Connect normally open relays to the GPIO pins:
- Relay 1: Connect to GPIO 19
- Relay 2: Connect to GPIO 21
- Each relay should be wired in parallel with the existing garage door pushbutton

## Operation

1. **Remote Control**: Use the ESP RainMaker mobile app to control each door independently
2. **Local Control**: Press the boot button to toggle Door 1
3. **Pulse Behavior**: When a door is activated, the GPIO goes LOW for 0.5 seconds, then returns to HIGH
4. **Relay Operation**: The LOW pulse closes the relay, simulating a button press on the garage door controller

## Configuration

The GPIO pins can be modified in `main/app_priv.h`:
```c
#define GARAGE_DOOR_1_GPIO    19
#define GARAGE_DOOR_2_GPIO    21
#define RELAY_PULSE_DURATION_MS  500
```

## Building and Flashing

1. Set up ESP-IDF environment
2. Configure the project: `idf.py menuconfig`
3. Build the project: `idf.py build`
4. Flash to ESP32: `idf.py flash monitor`

## RainMaker Setup

1. Flash the firmware to your ESP32
2. Follow the ESP RainMaker app setup process
3. The device will appear as "Garage Door Opener" with two separate switches
4. Each switch controls one garage door independently
