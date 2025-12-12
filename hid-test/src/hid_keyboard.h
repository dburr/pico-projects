#pragma once
#include <stdint.h>

#define HID_KEY_A      0x04
#define HID_KEY_SPACE  0x2C
#define HID_KEY_X      0x1B
#define HID_MOD_ALT    0x04

void hid_keyboard_init();
void hid_keyboard_send_key(uint8_t keycode);
void hid_keyboard_send_combo(uint8_t modifier, uint8_t keycode);
