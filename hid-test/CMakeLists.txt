cmake_minimum_required(VERSION 3.13)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/lib/tinyusb/hw/bsp/family_support.cmake)

project(rp2040-macropad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(macropad
    src/main.cpp
    src/hid_keyboard.cpp
    src/gpio_buttons.cpp
    src/usb_descriptors.c
)

# Add include directory for tusb_config.h
target_include_directories(macropad PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

pico_enable_stdio_usb(macropad 0)
pico_enable_stdio_uart(macropad 0)

# Pull in USB and hardware libraries
target_link_libraries(macropad 
    pico_stdlib 
    tinyusb_device
    tinyusb_board
)

# Create UF2 file for flashing
pico_add_extra_outputs(macropad)
