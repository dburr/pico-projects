# EMU1351 - Commodore 1351 Mouse Emulator

A Raspberry Pi Pico-based USB mouse host that will eventually emulate a Commodore 1351 mouse for use with Commodore 64 computers.

## Current Status: Stub Implementation

This initial version configures the Pico as a USB OTG host and reads data from a USB mouse, printing movement and button data via UART for debugging.

## Hardware Requirements

- Raspberry Pi Pico
- USB OTG adapter/cable (USB Micro or use a breakout board)
- USB Mouse
- FTDI USB-to-Serial adapter (for debug output)
- 2 LEDs with appropriate resistors (optional, for status indication)

## Pin Configuration

| GPIO Pin | Function | Description |
|----------|----------|-------------|
| GPIO 0 | UART TX | Debug output to FTDI adapter |
| GPIO 1 | UART RX | Debug input from FTDI adapter |
| GPIO 16 | LED | Mouse detected/connected indicator (ON = mouse OK) |
| GPIO 17 | LED | Error/no mouse indicator (ON = error/no mouse) |

## UART Settings

- Baud Rate: 115200
- Data Bits: 8
- Parity: None
- Stop Bits: 1

## Building the Project

1. Ensure you have the Pico SDK installed and `PICO_SDK_PATH` environment variable set:
   ```bash
   export PICO_SDK_PATH=/path/to/pico-sdk
   ```

2. Create a build directory and run CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Flash the resulting `emu1351.uf2` file to your Pico:
   - Hold the BOOTSEL button while connecting the Pico to your computer
   - Copy `emu1351.uf2` to the mounted RPI-RP2 drive

## Usage

1. Connect your FTDI adapter to GPIO 0 (TX) and GPIO 1 (RX), with ground
2. Connect the LEDs to GPIO 16 and GPIO 17 (with appropriate current-limiting resistors)
3. Connect a USB mouse via a USB OTG adapter to the Pico's USB port
4. Open a serial terminal (115200 baud) to view the debug output
5. Move the mouse or click buttons to see the output

## Example Output

```
=== Commodore 1351 Mouse Emulator ===
USB Mouse Host Mode - Stub Implementation
UART Debug Output at 115200 baud
Waiting for USB mouse...

[USB] Device mounted, address = 1
[HID] Device mounted: address = 1, instance = 0
[MOUSE] Mouse detected!
[MOUSE] X:   3 Y:   0 Wheel:   0 Buttons:
[MOUSE] X:   5 Y:  -2 Wheel:   0 Buttons:
[MOUSE] X:   0 Y:   0 Wheel:   0 Buttons: LEFT
```

## LED Status Indicators

- **GPIO 16 (Mouse OK)**: Illuminated when a USB mouse is successfully detected and connected
- **GPIO 17 (Error)**: Illuminated when no mouse is detected or there's an error

## Next Steps

Future development will include:
- Commodore 1351 protocol implementation
- C64 control port output (potentiometer emulation)
- Button mapping to C64 joystick buttons
- Calibration and sensitivity adjustment
- Configuration storage

## License

This project is provided as-is for educational and hobby purposes.
