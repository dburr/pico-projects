cmake_minimum_required(VERSION 3.13)

# Initialize the SDK
include(pico_sdk_import.cmake)

project(emu1351 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(emu1351
    main.c
)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(emu1351 
    pico_stdlib
    tinyusb_host
    tinyusb_board
)

# Enable USB host mode
target_compile_definitions(emu1351 PRIVATE
    CFG_TUSB_RHPORT0_MODE=OPT_MODE_HOST
)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(emu1351)

# Enable UART output, disable USB output
pico_enable_stdio_uart(emu1351 1)
pico_enable_stdio_usb(emu1351 0)
