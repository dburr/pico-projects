#include <stdio.h>
#include "pico/stdlib.h"

#define DELAY_MS 1000

int main() {
  const uint led_pin = 25;
  
  // initialize LED pin
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  
  // Initialize uart
  stdio_init_all();
  
  // loop forever
  while(true) {
    // blink LED
    printf("Blinking!\r\n");
    gpio_put(led_pin, true);
    sleep_ms(DELAY_MS);
    gpio_put(led_pin, false);
    sleep_ms(DELAY_MS);
  }
}