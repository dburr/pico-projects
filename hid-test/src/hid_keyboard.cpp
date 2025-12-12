#include "hid_keyboard.h"
#include "tusb.h"
#include "pico/stdlib.h"

/**
 * @brief Initialize the HID keyboard
 * 
 */
void hid_keyboard_init() {
    tusb_init();
}

/**
 * @brief Send a key press
 * 
 * @param keycode The keycode to send
 */
void hid_keyboard_send_key(uint8_t keycode) {
    if (!tud_hid_ready()) return;
    uint8_t keycodes[6] = {keycode, 0, 0, 0, 0, 0};
    tud_hid_keyboard_report(0, 0, keycodes);
}

/**
 * @brief Send a key combination (modifier + key)
 * 
 * @param modifier The modifier key (e.g., KEY_MOD_LCTRL)
 * @param keycode The keycode to send
 */
void hid_keyboard_send_combo(uint8_t modifier, uint8_t keycode) {
    if (!tud_hid_ready()) return;
    uint8_t keycodes[6] = {keycode, 0, 0, 0, 0, 0};
    tud_hid_keyboard_report(0, modifier, keycodes);
}

// TinyUSB HID callbacks
/**
 * @brief Callback function when a HID report is requested
 * 
 * @param instance The HID instance
 * @param report_id The report ID
 * @param report_type The type of report
 * @param buffer The buffer to fill with the report data
 * @param reqlen The length of the requested report
 * @return uint16_t The length of the report
 */
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {
    return 0;
}

/**
 * @brief Callback function when a HID report is set
 * 
 * @param instance The HID instance
 * @param report_id The report ID
 * @param report_type The type of report
 * @param buffer The buffer containing the report data
 * @param bufsize The size of the buffer
 */
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {
}
