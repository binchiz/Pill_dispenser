# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Set board type because we are building for PicoW
set(PICO_BOARD pico_w)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(PillDispenser C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
)


# Add the src and test directory
add_subdirectory(test)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src)

# Create a library from your source files (excluding main.c)
add_library(${PROJECT_NAME}_lib STATIC
        # drivers
        src/drivers/button.c
        src/drivers/eeprom.c
        src/drivers/led.c
        src/drivers/lora.c
        src/drivers/piezo_sensor.c
        #src/drivers/stepper_motor.c
        src/drivers/isr.c
        src/drivers/dispenser.c

        # utils
        src/utils/debug.c

        # main logic
        #src/motor_control.c
        src/lora_msg.c
        src/storage.c
        src/dispenser_control.c
)


# Add the main executable
add_executable(${PROJECT_NAME}
        # main
        src/main.c
)

# Link the necessary Pico SDK libraries to the static library
target_link_libraries(${PROJECT_NAME}_lib
        pico_stdlib
        hardware_pwm
        hardware_gpio
        hardware_i2c
        hardware_uart
)

# Link the static library to the main executable
target_link_libraries(${PROJECT_NAME}
        ${PROJECT_NAME}_lib
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Disable USB output, enable UART output
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
