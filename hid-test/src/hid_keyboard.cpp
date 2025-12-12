#include "hid_keyboard.h"
#include "tusb.h"
#include "pico/stdlib.h"

void hid_keyboard_init() {
    tusb_init();
}

void hid_keyboard_send_key(uint8_t keycode) {
    if (!tud_hid_ready()) return;
    uint8_t keycodes[6] = {keycode, 0, 0, 0, 0, 0};
    tud_hid_keyboard_report(1, 0, keycodes);
    sleep_ms(20);
    tud_hid_keyboard_report(1, 0, NULL); // Release
}

void hid_keyboard_send_combo(uint8_t modifier, uint8_t keycode) {
    if (!tud_hid_ready()) return;
    uint8_t keycodes[6] = {keycode, 0, 0, 0, 0, 0};
    tud_hid_keyboard_report(1, modifier, keycodes);
    sleep_ms(20);
    tud_hid_keyboard_report(1, 0, NULL); // Release
}

// TinyUSB HID callbacks
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {
    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {
}
