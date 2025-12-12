#pragma once
#include <stdint.h>
#include "pico/types.h"

void gpio_buttons_init(const uint* pins, uint num);
bool gpio_button_pressed(uint idx);
