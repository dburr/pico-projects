#pragma once
#include <stdint.h>

// HID scan codes: https://gist.github.com/MightyPork/6da26e382a7ad91b5496ee55fdc73db2
// moved to hid_keys.h

void hid_keyboard_init();
void hid_keyboard_send_key(uint8_t keycode);
void hid_keyboard_send_combo(uint8_t modifier, uint8_t keycode);
