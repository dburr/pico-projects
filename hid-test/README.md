# RP2040 Macro Pad (USB HID Keyboard)

This project is a C++ firmware for the Raspberry Pi Pico (RP2040) that acts as a USB HID keyboard (macro pad). It uses GPIO pins for button inputs and sends keyboard events to the host PC.

## Features
- Acts as a USB keyboard (HID device)
- Maps GPIO buttons to key events (single keys and combos)
- Example mappings: 'A', Spacebar, Alt+X

## Hardware Setup
- Connect buttons to GPIO pins 2, 3, and 4 (changeable in `main.cpp`)
- Use pull-up resistors (internal pull-ups enabled)
- Button press grounds the pin

## Build Instructions
1. Install the Raspberry Pi Pico SDK and toolchain
2. Clone this repository and open a terminal in the project root
3. Run:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
4. Flash the Pico:
   - Hold BOOTSEL button, plug in Pico
   - Copy `macropad.uf2` from `build/` to Pico's USB drive

## File Overview
- `src/main.cpp`: Main loop, button handling, HID send logic
- `src/gpio_buttons.cpp/h`: GPIO button setup and state
- `src/hid_keyboard.cpp/h`: USB HID keyboard logic
- `CMakeLists.txt`: Build configuration

## Customization
- Change GPIO pins in `main.cpp`
- Map buttons to different keys or combos in `main.cpp` and `hid_keyboard.h`

## References
- [Pico SDK USB HID Example](https://github.com/raspberrypi/pico-examples/tree/master/usb/hid)

---
For more details, see `.github/copilot-instructions.md` for AI agent guidance.
