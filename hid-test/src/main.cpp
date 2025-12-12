#include "pico/stdlib.h"
#include "tusb.h"
#include "gpio_buttons.h"
#include "hid_keyboard.h"

const uint BUTTON_PINS[] = {2, 3, 4}; // GPIOs for buttons
const uint NUM_BUTTONS = sizeof(BUTTON_PINS) / sizeof(BUTTON_PINS[0]);

static bool button_prev_state[3] = {false};
static uint32_t button_last_change[3] = {0};
const uint32_t DEBOUNCE_MS = 200;

int main() {
    stdio_init_all();
    gpio_buttons_init(BUTTON_PINS, NUM_BUTTONS);
    hid_keyboard_init();

    while (true) {
        tud_task(); // TinyUSB device task - must run frequently!
        
        uint32_t now = to_ms_since_boot(get_absolute_time());
        
        for (uint i = 0; i < NUM_BUTTONS; ++i) {
            bool pressed = gpio_button_pressed(i);
            
            // Detect button press (transition from not pressed to pressed)
            if (pressed && !button_prev_state[i]) {
                // Check debounce time
                if (now - button_last_change[i] > DEBOUNCE_MS) {
                    // Send key press
                    switch (i) {
                        case 0:
                            hid_keyboard_send_key(HID_KEY_A);
                            break;
                        case 1:
                            hid_keyboard_send_key(HID_KEY_SPACE);
                            break;
                        case 2:
                            hid_keyboard_send_combo(HID_MOD_ALT, HID_KEY_X);
                            break;
                    }
                    button_last_change[i] = now;
                }
            }
            // Detect button release
            else if (!pressed && button_prev_state[i]) {
                // Send key release
                if (tud_hid_ready()) {
                    tud_hid_keyboard_report(0, 0, NULL);
                }
                button_last_change[i] = now;
            }
            
            button_prev_state[i] = pressed;
        }
        
        sleep_ms(1); // Short delay, keeps USB responsive
    }
}
