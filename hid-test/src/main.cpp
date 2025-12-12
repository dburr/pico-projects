#include "pico/stdlib.h"
#include "tusb.h"
#include "gpio_buttons.h"
#include "hid_keyboard.h"

const uint BUTTON_PINS[] = {2, 3, 4}; // GPIOs for buttons
const uint NUM_BUTTONS = sizeof(BUTTON_PINS) / sizeof(BUTTON_PINS[0]);

int main() {
    stdio_init_all();
    gpio_buttons_init(BUTTON_PINS, NUM_BUTTONS);
    hid_keyboard_init();

    while (true) {
        tud_task(); // TinyUSB device task
        for (uint i = 0; i < NUM_BUTTONS; ++i) {
            if (gpio_button_pressed(i)) {
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
                sleep_ms(200); // Simple debounce
            }
        }
        sleep_ms(10);
    }
}
