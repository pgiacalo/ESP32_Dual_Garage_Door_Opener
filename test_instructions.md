# Testing Instructions

## Pre-Flash Testing

Before connecting to garage doors, test the relay functionality:

### Hardware Test Setup
1. Connect relays to GPIO 19 and GPIO 21
2. Connect multimeter to relay contacts (COM and NO)
3. Power up ESP32

### Test Procedure
1. **Default State Check**: Verify relays are open (infinite resistance between COM and NO)
2. **Door 1 Test**: 
   - Press boot button or use RainMaker app
   - Verify GPIO 19 goes LOW for 0.5 seconds
   - Verify relay closes during pulse
3. **Door 2 Test**:
   - Use RainMaker app only (boot button doesn't control Door 2)
   - Verify GPIO 21 goes LOW for 0.5 seconds
   - Verify relay closes during pulse

## Serial Monitor Output

Expected log messages:
```
I (xxxx) app_driver: Door 1: Starting pulse (GPIO 19 LOW for 500 ms)
I (xxxx) app_driver: Door 1: Pulse completed
I (xxxx) app_driver: Door 2: Starting pulse (GPIO 21 LOW for 500 ms)
I (xxxx) app_driver: Door 2: Pulse completed
```

## RainMaker App Testing

1. **Device Discovery**: Device should appear as "Garage Door Opener"
2. **Two Switches**: Should see two separate switches named "Garage Door 1" and "Garage Door 2"
3. **Independent Control**: Each switch should control its respective door independently
4. **State Feedback**: Switch should return to OFF position after activation

## Troubleshooting

### Relays Not Closing
- Check GPIO voltage levels with multimeter
- Verify relay module power supply
- Check relay module input voltage requirements

### RainMaker Not Working
- Verify WiFi connection
- Check device provisioning
- Ensure ESP RainMaker app is properly configured

### Pulse Too Short/Long
- Modify `RELAY_PULSE_DURATION_MS` in `app_priv.h`
- Test with different garage door controllers
