#include "gpio_buttons.h"
#include "pico/stdlib.h"

static const uint* button_pins;
static uint button_count;

void gpio_buttons_init(const uint* pins, uint num) {
    button_pins = pins;
    button_count = num;
    for (uint i = 0; i < num; ++i) {
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_IN);
        gpio_pull_up(pins[i]);
    }
}

bool gpio_button_pressed(uint idx) {
    if (idx >= button_count) return false;
    return gpio_get(button_pins[idx]) == 0;
}
