#include "gpio_buttons.h"
#include "pico/stdlib.h"

static const uint* button_pins;
static uint button_count;

/**
 * @brief Initialize GPIO buttons
 * 
 * @param pins Array of GPIO pins
 * @param num Number of buttons
 */
void gpio_buttons_init(const uint* pins, uint num) {
    button_pins = pins;
    button_count = num;
    for (uint i = 0; i < num; ++i) {
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_IN);
        gpio_pull_up(pins[i]);
    }
}

/**
 * @brief Check if a GPIO button is pressed
 * 
 * @param idx Index of the button
 * @return true If the button is pressed
 * @return false If the button is not pressed
 */
bool gpio_button_pressed(uint idx) {
    if (idx >= button_count) return false;
    return gpio_get(button_pins[idx]) == 0;
}
