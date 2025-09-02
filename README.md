# ESP32 Dual Garage Door Opener

This project creates an ESP32-based garage door opener using Espressif RainMaker that can control two separate garage doors remotely. The implementation is based on the ESP RainMaker switch example with minimal modifications to follow the established patterns.

**Tested and verified on ESP32-C6 development boards.**

## 📚 **Important: Complete Development Guide**

**For a comprehensive understanding of this project, including the complete development process, technical decisions, lessons learned, and future enhancements, please read:**

**[ESP32_RainMaker_Development_Case_Study.md](ESP32_RainMaker_Development_Case_Study.md)**

This case study documents the complete transformation from the Espressif switch example to this production-ready garage door opener, including:
- **Complete code changes** and technical implementation
- **Safety features** and security considerations
- **Platform compatibility** testing (iOS vs Android)
- **Lessons learned** from real development challenges
- **Alternative approaches** considered and trade-offs made
- **Future enhancement** possibilities
- **Development environment** setup and debugging techniques

*This README provides the essential information to get started, while the case study offers deep technical insights and development guidance.*

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

## Hardware Setup

### GPIO Connections
- **Door 1**: GPIO 15 (default HIGH, goes LOW for 0.5s when activated)
- **Door 2**: GPIO 19 (default HIGH, goes LOW for 0.5s when activated)
- **Boot Button**: GPIO 0 (for local testing and WiFi reset)

### Relay Wiring
Connect normally open relays to the GPIO pins:
- Relay 1: Connect to GPIO 15
- Relay 2: Connect to GPIO 19
- Each relay should be wired in parallel with the existing garage door pushbutton

**For detailed wiring instructions, see [WIRING.md](WIRING.md)**

## Implementation Details

### GPIO Configuration
- **Door 1**: GPIO 15 (configurable in `app_priv.h`)
- **Door 2**: GPIO 19 (configurable in `app_priv.h`)
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

## Operation

1. **Remote Control**: Use the ESP RainMaker mobile app to control each door independently
2. **Local Control**: Press the boot button to toggle Door 1
3. **Pulse Behavior**: When a door is activated, the GPIO goes LOW for 0.5 seconds, then returns to HIGH
4. **Relay Operation**: The LOW pulse closes the relay, simulating a button press on the garage door controller

### Operation Sequence

1. **Idle State**: 
   - GPIO 15 and GPIO 19 are HIGH
   - Relays are open (NO contacts disconnected)
   - Garage doors remain in current state

2. **Door Activation**:
   - GPIO pin goes LOW for 500ms
   - Relay closes (NO contacts connect)
   - Garage door controller receives button press signal
   - GPIO returns to HIGH
   - Relay opens (NO contacts disconnect)

3. **Door Movement**:
   - If door is closed → opens
   - If door is open → closes
   - If door is moving → stops

## Configuration

The GPIO pins can be modified in `main/app_priv.h`:
```c
#define GARAGE_DOOR_1_GPIO    15
#define GARAGE_DOOR_2_GPIO    19
#define RELAY_PULSE_DURATION_MS  500
```

### Customization Options

#### GPIO Pins
Modify in `main/app_priv.h`:
```c
#define GARAGE_DOOR_1_GPIO    15
#define GARAGE_DOOR_2_GPIO    19
```

#### Pulse Duration
Modify in `main/app_priv.h`:
```c
#define RELAY_PULSE_DURATION_MS  500
```

#### Device Names
Modify in `main/app_main.c`:
```c
door1_device = esp_rmaker_device_create("Garage Door 1", ESP_RMAKER_DEVICE_SWITCH, NULL);
door2_device = esp_rmaker_device_create("Garage Door 2", ESP_RMAKER_DEVICE_SWITCH, NULL);
```

## Building and Flashing

1. Set up ESP-IDF environment
2. Configure the project: `idf.py menuconfig`
3. Build the project: `idf.py build`
4. Flash to ESP32: `idf.py flash monitor`

### Build Commands
```bash
# Clean build
idf.py fullclean

# Build project
idf.py build

# Flash to device
idf.py flash

# Monitor output
idf.py monitor

# Combined build and flash
idf.py build flash monitor
```

## RainMaker Setup

1. Flash the firmware to your ESP32
2. Follow the ESP RainMaker app setup process
3. The device will appear as "Garage Door Opener" with two separate switches
4. Each switch controls one garage door independently

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
└── ESP32_RainMaker_Development_Case_Study.md # Development guide
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
- **Startup Protection**: Multiple safety layers prevent accidental activation during boot
- **Testing**: Always test with multimeter before connecting to garage door system
- **Fuses**: Consider adding appropriate fuses for additional protection
- **Voltage**: Ensure relay module voltage matches ESP32 output (3.3V or 5V)
- **Current**: Verify relay can handle garage door controller current requirements

## Testing

- **Hardware Test**: Use multimeter to verify relay operation
- **Software Test**: Monitor serial output for pulse messages
- **App Test**: Verify RainMaker app control functionality
- **Integration Test**: Test with actual garage door controllers

### Testing Procedures
For detailed testing instructions, see [test_instructions.md](test_instructions.md)

## Troubleshooting

- **Door doesn't respond**: Check relay connections and garage door button terminals
- **ESP32 resets**: Verify power supply can handle relay module current draw
- **Intermittent operation**: Check for loose connections or poor grounding
- **Wrong door activates**: Verify GPIO pin assignments match relay connections

## Hardware Requirements

### ESP32 Development Board:
- **ESP32-C6**: Tested and verified (primary development platform)
- **ESP32-WROOM-32D**: Compatible (GPIO 15/19 available)
- **ESP32-S3-WROOM-1**: Compatible (GPIO 15/19 available)

### Power Supply:
- **USB Power**: Works directly for development and testing
- **External 5V DC**: Requires 1.5F supercapacitor for power stability
  - **Voltage**: 5V ±5% (4.75V - 5.25V)
  - **Current**: Minimum 1A, recommended 2A
  - **Supercapacitor**: 1.5F, 5.5V or higher rating

### Relay Module:
- **Type**: Dual-channel normally open (NO) relay module
- **Control Voltage**: 5V or 3.3V (check your module)
- **Contact Rating**: 10A @ 120V AC (for garage door controller)

## Platform Compatibility

### iOS (Working Correctly):
- ✅ Sliders return to OFF after 0.5 seconds
- ✅ Momentary button behavior as intended
- ✅ Reliable relay activation
- ✅ Proper parameter synchronization

### Android (Functional with UI Quirk):
- ✅ Relays work perfectly
- ✅ Button presses trigger garage doors
- ✅ Top-level switches work correctly
- ⚠️ Detail view sliders stay ON (platform-specific UI sync issue)

## Usage Instructions

1. **Hardware Setup**: Connect relays to GPIO 15 and 19
2. **Power Supply**: Use USB for development or external 5V with 1.5F supercapacitor
3. **Build**: Run `./build.sh` or `idf.py build`
4. **Flash**: Run `./build.sh flash-monitor`
5. **Configure**: Use ESP RainMaker app to provision device
6. **Control**: Use app to control doors remotely

This implementation provides a robust, safe, and user-friendly solution for remote garage door control using ESP32 and RainMaker technology.
