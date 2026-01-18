/*
 * Commodore 1351 Mouse Emulator - USB Mouse Host Stub
 * 
 * Raspberry Pi Pico configured as USB OTG Host to read USB mouse data.
 * Eventually this will emulate a Commodore 1351 mouse for use with C64.
 * 
 * Pin Configuration:
 * - GPIO 0 (UART0 TX) - Debug UART output (connect to FTDI RX)
 * - GPIO 1 (UART0 RX) - Debug UART input (connect to FTDI TX)
 * - GPIO 16 (JOY_UP) - Joystick Up (also used for RMB)
 * - GPIO 17 (JOY_DOWN) - Joystick Down
 * - GPIO 13 (JOY_LEFT) - Joystick Left
 * - GPIO 14 (JOY_RIGHT) - Joystick Right
 * - GPIO 18 (JOY_BUTTON) - Joystick Button (used for LMB)
 * - GPIO 19 (POT_X) - Potentiometer X-axis
 * - GPIO 15 (POT_Y) - Potentiometer Y-axis
 * - GPIO 4 - LED for mouse detected/connected
 * - GPIO 5 - LED for error/no mouse detected
 * - GPIO 2 - Mode select: Proportional mode (input with pull-up)
 * - GPIO 3 - Mode select: Joystick mode (input with pull-up)
 */

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "tusb.h"
#include "hardware/uart.h"

// Pin definitions
#define UART_ID uart0
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define UART_BAUD_RATE 115200

#define LED_MOUSE_OK 4
#define LED_ERROR 5

#define MODE_PROPORTIONAL 2
#define MODE_JOYSTICK 3

#define JOY_UP 16
#define JOY_DOWN 17
#define JOY_LEFT 13
#define JOY_RIGHT 14
#define JOY_BUTTON 18

#define POT_X 19
#define POT_Y 15

// Mouse state tracking
static bool mouse_mounted = false;
static uint8_t mouse_dev_addr = 0;
static uint8_t mouse_instance = 0;

//--------------------------------------------------------------------+
// Function Prototypes
//--------------------------------------------------------------------+
void init_hardware(void);
void update_leds(void);

//--------------------------------------------------------------------+
// Hardware Initialization
//--------------------------------------------------------------------+
void init_hardware(void) {
    // Initialize UART0 for debug output
    uart_init(UART_ID, UART_BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // Initialize LED pins
    gpio_init(LED_MOUSE_OK);
    gpio_set_dir(LED_MOUSE_OK, GPIO_OUT);
    gpio_put(LED_MOUSE_OK, 0);
    
    gpio_init(LED_ERROR);
    gpio_set_dir(LED_ERROR, GPIO_OUT);
    gpio_put(LED_ERROR, 1);  // Error LED on by default (no mouse)
    
    // Initialize mode selection pins as inputs with pull-ups
    gpio_init(MODE_PROPORTIONAL);
    gpio_set_dir(MODE_PROPORTIONAL, GPIO_IN);
    gpio_pull_up(MODE_PROPORTIONAL);
    
    gpio_init(MODE_JOYSTICK);
    gpio_set_dir(MODE_JOYSTICK, GPIO_IN);
    gpio_pull_up(MODE_JOYSTICK);
        // Initialize joystick pins as inputs with pull-ups
    gpio_init(JOY_UP);
    gpio_set_dir(JOY_UP, GPIO_IN);
    gpio_pull_up(JOY_UP);
    
    gpio_init(JOY_DOWN);
    gpio_set_dir(JOY_DOWN, GPIO_IN);
    gpio_pull_up(JOY_DOWN);
    
    gpio_init(JOY_LEFT);
    gpio_set_dir(JOY_LEFT, GPIO_IN);
    gpio_pull_up(JOY_LEFT);
    
    gpio_init(JOY_RIGHT);
    gpio_set_dir(JOY_RIGHT, GPIO_IN);
    gpio_pull_up(JOY_RIGHT);
    
    gpio_init(JOY_BUTTON);
    gpio_set_dir(JOY_BUTTON, GPIO_IN);
    gpio_pull_up(JOY_BUTTON);
    
    // Initialize potentiometer pins as inputs with pull-ups
    gpio_init(POT_X);
    gpio_set_dir(POT_X, GPIO_IN);
    gpio_pull_up(POT_X);
    
    gpio_init(POT_Y);
    gpio_set_dir(POT_Y, GPIO_IN);
    gpio_pull_up(POT_Y);
        printf("\n\n=== Commodore 1351 Mouse Emulator ===\n");
    printf("USB Mouse Host Mode - Stub Implementation\n");
    printf("UART Debug Output at %d baud\n", UART_BAUD_RATE);
    printf("Waiting for USB mouse...\n\n");
}

//--------------------------------------------------------------------+
// LED Status Updates
//--------------------------------------------------------------------+
void update_leds(void) {
    if (mouse_mounted) {
        gpio_put(LED_MOUSE_OK, 1);
        gpio_put(LED_ERROR, 0);
    } else {
        gpio_put(LED_MOUSE_OK, 0);
        gpio_put(LED_ERROR, 1);
    }
}

//--------------------------------------------------------------------+
// TinyUSB Host Callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tuh_mount_cb(uint8_t dev_addr) {
    printf("[USB] Device mounted, address = %d\n", dev_addr);
}

// Invoked when device is unmounted
void tuh_umount_cb(uint8_t dev_addr) {
    printf("[USB] Device unmounted, address = %d\n", dev_addr);
    
    if (dev_addr == mouse_dev_addr) {
        mouse_mounted = false;
        mouse_dev_addr = 0;
        mouse_instance = 0;
        printf("[MOUSE] Mouse disconnected\n");
        update_leds();
    }
}

// Invoked when a HID device is mounted
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len) {
    (void)desc_report;
    (void)desc_len;
    
    printf("[HID] Device mounted: address = %d, instance = %d\n", dev_addr, instance);
    
    // Get the HID interface protocol
    uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
    
    if (itf_protocol == HID_ITF_PROTOCOL_MOUSE) {
        printf("[MOUSE] Mouse detected!\n");
        mouse_mounted = true;
        mouse_dev_addr = dev_addr;
        mouse_instance = instance;
        update_leds();
        
        // Request to receive reports
        if (!tuh_hid_receive_report(dev_addr, instance)) {
            printf("[ERROR] Failed to request report\n");
        }
    } else {
        printf("[HID] Non-mouse HID device (protocol = %d)\n", itf_protocol);
    }
}

// Invoked when a HID device is unmounted
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
    printf("[HID] Device unmounted: address = %d, instance = %d\n", dev_addr, instance);
    
    if (dev_addr == mouse_dev_addr && instance == mouse_instance) {
        mouse_mounted = false;
        mouse_dev_addr = 0;
        mouse_instance = 0;
        printf("[MOUSE] Mouse disconnected\n");
        update_leds();
    }
}

// Process mouse report
static void process_mouse_report(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) {
    // Standard boot protocol mouse report is 3-4 bytes:
    // Byte 0: Button mask
    // Byte 1: X movement (signed)
    // Byte 2: Y movement (signed)
    // Byte 3: Wheel movement (optional, signed)
    
    if (len < 3) {
        printf("[MOUSE] Report too short (%d bytes)\n", len);
        return;
    }
    
    uint8_t buttons = report[0];
    int8_t x = (int8_t)report[1];
    int8_t y = (int8_t)report[2];
    int8_t wheel = (len >= 4) ? (int8_t)report[3] : 0;
    
    // Only print if there's actual movement or button state
    if (x != 0 || y != 0 || wheel != 0 || buttons != 0) {
        printf("[MOUSE] X:%4d Y:%4d Wheel:%4d Buttons:", x, y, wheel);
        
        if (buttons & 0x01) printf(" LEFT");
        if (buttons & 0x02) printf(" RIGHT");
        if (buttons & 0x04) printf(" MIDDLE");
        
        printf("\n");
    }
}

// Invoked when received report from device via interrupt endpoint
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) {
    if (dev_addr == mouse_dev_addr && instance == mouse_instance) {
        process_mouse_report(dev_addr, instance, report, len);
    }
    
    // Continue to request to receive reports
    tuh_hid_receive_report(dev_addr, instance);
}

//--------------------------------------------------------------------+
// Main Function
//--------------------------------------------------------------------+
int main(void) {
    // Initialize hardware
    init_hardware();
    
    // Initialize TinyUSB host stack
    tusb_init();
    
    // Main loop
    while (1) {
        // TinyUSB host task
        tuh_task();
        
        // Small delay to prevent busy-waiting
        sleep_ms(1);
    }
    
    return 0;
}
