# Build Guide - ESP32 Dual Garage Door Opener

This guide provides step-by-step instructions for building the ESP32 Dual Garage Door Opener project, including all recent fixes and compatibility requirements.

## Prerequisites

### Required Software
- **ESP-IDF v5.5** or later (tested with v5.5.1)
- **CMake 3.10** or later (required for compatibility)
- **Python 3.12** (recommended, tested with 3.12.9)
- **Git** (for component management)

### Hardware Requirements
- **ESP32-C6** development board (primary target, tested and verified)
- **ESP32-WROOM-32D** or **ESP32-S3-WROOM-1** (compatible alternatives)
- **Dual-channel relay module** (5V or 3.3V, normally open contacts)
- **Power supply**: USB for development, or 5V DC with 1.5F supercapacitor for production

## Environment Setup

### 1. Install ESP-IDF
```bash
# Clone ESP-IDF (if not already installed)
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
git checkout v5.5.1

# Install ESP-IDF
./install.sh esp32c6

# Source the environment
source export.sh
```

### 2. Verify Installation
```bash
# Check ESP-IDF version
idf.py --version

# Check CMake version (should be 3.10+)
cmake --version

# Check Python version
python3 --version
```

## Project Setup

### 1. Clone the Project
```bash
git clone <repository-url>
cd ESP32_Dual_Garage_Door_Opener
```

### 2. Set Target
```bash
# Set target to ESP32-C6 (recommended)
idf.py set-target esp32c6

# Alternative targets (if using different board)
# idf.py set-target esp32
# idf.py set-target esp32s3
```

### 3. Configure Project
```bash
# Open configuration menu
idf.py menuconfig

# Key configurations to verify:
# - Partition Table: Custom (partitions_4mb_optimised.csv)
# - Flash Size: 4MB
# - WiFi: Enabled
# - RainMaker: Enabled
```

## Building the Project

### Clean Build (Recommended)
```bash
# Full clean to ensure fresh build
idf.py fullclean

# Build the project
idf.py build
```

### Quick Build (if no changes to configuration)
```bash
# Standard build
idf.py build
```

### Build with Monitoring
```bash
# Build and monitor output
idf.py build monitor
```

## Recent Fixes Applied

This project has been updated to resolve all major build warnings and compatibility issues:

### ✅ Fixed Issues
1. **CMake Version**: Updated from 3.5 to 3.10 for future compatibility
2. **Kconfig Symbols**: Removed deprecated `MBEDTLS_DYNAMIC_FREE_PEER_CERT`
3. **Configuration**: Cleaned up deprecated RainMaker configuration comments
4. **Managed Components**: Updated `esp-serial-flasher` CMake version

### Expected Build Output
A successful build should show:
- ✅ No CMake deprecation warnings
- ✅ No unknown Kconfig symbol warnings
- ✅ Clean configuration loading
- ✅ Successful binary generation

### Remaining Warnings (Expected)
The build may still show these warnings, which are **normal and expected**:
- FreeRTOS header deprecation (from managed components)
- RMT driver deprecation (from ESP RainMaker examples)

These warnings are from third-party components and will be resolved in future ESP-IDF updates.

## Flashing the Firmware

### 1. Connect Hardware
- Connect ESP32 to computer via USB
- Ensure proper drivers are installed

### 2. Flash Firmware
```bash
# Flash with monitoring
idf.py flash monitor

# Flash only (no monitoring)
idf.py flash

# Flash with specific port
idf.py -p /dev/ttyUSB0 flash monitor
```

### 3. Verify Flash
```bash
# Check flash size and partition table
idf.py size

# Expected output should show:
# - Binary size: ~1.8MB
# - Available space: ~7% free
# - Partition table: Custom 4MB optimized
```

## Configuration Files

### Key Configuration Files
- `sdkconfig.defaults` - Main project defaults
- `sdkconfig.defaults.esp32c6` - ESP32-C6 specific settings
- `partitions_4mb_optimised.csv` - Optimized partition table
- `CMakeLists.txt` - Build configuration (updated to CMake 3.10)

### Important Settings
```ini
# Flash configuration
CONFIG_ESPTOOLPY_FLASHSIZE_4MB=y
CONFIG_PARTITION_TABLE_CUSTOM=y
CONFIG_PARTITION_TABLE_CUSTOM_FILENAME="partitions_4mb_optimised.csv"

# Security features
CONFIG_ESP_RMAKER_USER_ID_CHECK=y
CONFIG_ESP_RMAKER_LOCAL_CTRL_AUTO_ENABLE=y
CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE=y

# mbedTLS (cleaned up deprecated symbols)
CONFIG_MBEDTLS_DYNAMIC_BUFFER=y
CONFIG_MBEDTLS_DYNAMIC_FREE_CONFIG_DATA=y
CONFIG_MBEDTLS_CERTIFICATE_BUNDLE_DEFAULT_CMN=y
```

## Troubleshooting

### Common Build Issues

#### 1. CMake Version Error
```
Error: CMake 3.10 or higher is required
```
**Solution**: Update CMake to version 3.10 or later

#### 2. ESP-IDF Version Mismatch
```
Error: ESP-IDF version not found
```
**Solution**: Source the ESP-IDF environment:
```bash
source ~/esp/esp-idf/export.sh
```

#### 3. Python Version Issues
```
Error: Python version not supported
```
**Solution**: Use Python 3.12 (recommended) or 3.8+

#### 4. Component Dependencies
```
Error: Component not found
```
**Solution**: Run full clean and rebuild:
```bash
idf.py fullclean
idf.py build
```

### Build Warnings

#### Expected Warnings (Safe to Ignore)
- FreeRTOS header deprecation warnings
- RMT driver deprecation warnings
- These are from managed components and will be fixed in future updates

#### Unexpected Warnings (Should Investigate)
- CMake deprecation warnings (should be fixed in this project)
- Unknown Kconfig symbols (should be fixed in this project)
- Compilation errors (need to fix)

## Hardware Setup

### GPIO Connections
- **Door 1**: GPIO 15 (configurable in `main/app_priv.h`)
- **Door 2**: GPIO 19 (configurable in `main/app_priv.h`)
- **Boot Button**: GPIO 0 (for local testing)

### Relay Module
- **Type**: Dual-channel normally open (NO) relay
- **Control Voltage**: 5V or 3.3V (check module specifications)
- **Contact Rating**: 10A @ 120V AC minimum

### Power Supply
- **Development**: USB power (5V, 500mA minimum)
- **Production**: External 5V DC with 1.5F supercapacitor
  - Voltage: 5V ±5% (4.75V - 5.25V)
  - Current: 1A minimum, 2A recommended

## Testing

### 1. Hardware Test
```bash
# Monitor serial output
idf.py monitor

# Expected output:
# - WiFi connection status
# - RainMaker initialization
# - Device registration
# - GPIO pulse messages when activated
```

### 2. Functionality Test
1. **Local Test**: Press boot button to toggle Door 1
2. **Remote Test**: Use ESP RainMaker app to control both doors
3. **Relay Test**: Use multimeter to verify relay operation

### 3. Integration Test
1. Connect relays to garage door controller
2. Test door operation via app
3. Verify safety features (pulse duration, fail-safe behavior)

## Development Tips

### Code Modifications
- GPIO pins: Modify in `main/app_priv.h`
- Pulse duration: Change `RELAY_PULSE_DURATION_MS`
- Device names: Update in `main/app_main.c`

### Debugging
- Use `idf.py monitor` for real-time debugging
- Check serial output for error messages
- Verify GPIO states with multimeter

### Performance Optimization
- Binary size: ~1.8MB (93% of available space)
- Boot time: ~3-5 seconds
- Response time: <1 second for door activation

## Support

### Documentation
- **Main README**: [README.md](README.md)
- **Wiring Guide**: [WIRING.md](WIRING.md)
- **Development Case Study**: [ESP32_RainMaker_Development_Case_Study.md](ESP32_RainMaker_Development_Case_Study.md)
- **Test Instructions**: [test_instructions.md](test_instructions.md)

### Common Issues
- Check hardware connections
- Verify power supply capacity
- Ensure proper relay module specifications
- Test with multimeter before connecting to garage door system

This build guide ensures users can successfully build and deploy the ESP32 Dual Garage Door Opener with all recent fixes and optimizations applied.
