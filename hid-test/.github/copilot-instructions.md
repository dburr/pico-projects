# Copilot Instructions for RP2040 USB HID Macro Pad

This project is a C++ firmware for the Raspberry Pi Pico (RP2040) that acts as a USB HID keyboard (macro pad). It uses GPIO pins for button inputs and sends keyboard events to the host PC.

## Architecture Overview
- **Main Components:**
  - `main.cpp`: Entry point, initializes GPIOs and USB HID stack.
  - `hid_keyboard.cpp/h`: Implements USB HID keyboard logic and key report formatting.
  - `gpio_buttons.cpp/h`: Handles button input (GPIO, pull-ups, debouncing).
  - `CMakeLists.txt`: Pico SDK build configuration.
- **Data Flow:**
  - Button press (GPIO) → Key event → HID report → USB host
- **Why:**
  - Direct GPIO-to-HID mapping for low-latency macro pad operation.

## Developer Workflows
- **Build:**
  - Use CMake and Pico SDK. Example:
    ```bash
    mkdir build && cd build
    cmake ..
    make
    ```
- **Flash:**
  - Copy resulting `.uf2` file to Pico's USB mass storage device.
- **Test:**
  - Press buttons, observe key events on host PC.

## Project-Specific Conventions
- **GPIOs:**
  - Buttons use GPIOs with internal pull-ups. Pressing grounds the pin.
  - GPIOs are defined in `main.cpp` (e.g., `const uint BUTTON_PINS[] = {2, 3, 4, 5}`)
- **Key Mapping:**
  - Each button maps to a key or key combo in `hid_keyboard.cpp`.
  - Multi-key combos (e.g., Alt+X) use modifier bits in HID report.
- **Debouncing:**
  - Simple software debounce in `gpio_buttons.cpp`.

## Integration Points
- **External Dependencies:**
  - Pico SDK (USB, hardware abstraction)
- **Cross-Component Communication:**
  - Button handler calls HID send function on state change.

## Examples
- **Single Key:**
  - Button 1 → "A"
  - Button 2 → Spacebar
- **Multi-Key Combo:**
  - Button 3 → Alt+X

## Key Files
- `main.cpp`: GPIO setup, main loop
- `hid_keyboard.cpp/h`: HID logic, key report
- `gpio_buttons.cpp/h`: Button state, debounce
- `CMakeLists.txt`: Build config

## References
- [Pico SDK USB HID Example](https://github.com/raspberrypi/pico-examples/tree/master/usb/hid)

---
Update this file as project structure evolves. Keep instructions concise and focused on actual codebase patterns.
