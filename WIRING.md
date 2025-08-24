# Wiring Diagram for Garage Door Opener

## ESP32 to Relay Connections

```
ESP32 GPIO 19 ────┐
                  ├─── Relay 1 (Normally Open)
                  └─── Garage Door 1 Controller Button

ESP32 GPIO 21 ────┐
                  ├─── Relay 2 (Normally Open)
                  └─── Garage Door 2 Controller Button
```

## Relay Specifications

- **Type**: Normally Open (NO) relay
- **Voltage**: 5V or 12V (depending on relay module)
- **Current**: Minimum 10mA switching current
- **Contact Rating**: 10A @ 120V AC (for garage door controller)

## Connection Details

### ESP32 Side
- **GPIO 19**: Connected to relay module input (usually marked "IN" or "SIG")
- **GPIO 21**: Connected to relay module input (usually marked "IN" or "SIG")
- **VCC**: 5V or 3.3V (depending on relay module)
- **GND**: Common ground

### Relay Side
- **Relay 1**: 
  - COM (Common) → Garage Door 1 button terminal 1
  - NO (Normally Open) → Garage Door 1 button terminal 2
- **Relay 2**:
  - COM (Common) → Garage Door 2 button terminal 1
  - NO (Normally Open) → Garage Door 2 button terminal 2

## Operation

1. **Default State**: GPIO pins are HIGH, relays are open (NO contacts)
2. **Activation**: When a door is triggered:
   - GPIO goes LOW for 0.5 seconds
   - Relay closes (NO contacts connect)
   - Garage door controller receives button press signal
   - GPIO returns to HIGH, relay opens

## Safety Notes

- Ensure proper isolation between ESP32 and garage door controller
- Use appropriate relay modules with optoisolation
- Test with multimeter before connecting to garage door system
- Consider adding fuses for additional protection
