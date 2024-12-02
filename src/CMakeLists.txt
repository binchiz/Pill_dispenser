
# Include directories
include_directories(${PROJECT_SOURCE_DIR}/hardware)
include_directories(${PROJECT_SOURCE_DIR}/utils)

# Create a library from your source files (excluding main.c)
add_library(${PROJECT_NAME}_lib STATIC
        # drivers
        drivers/button.c
        drivers/eeprom.c
        drivers/led.c
        drivers/lora.c
        drivers/piezo_sensor.c
        drivers/stepper_motor.c
        drivers/isr.c

        # utils
        utils/debug.c

        # main logic
        motor_control.c
        lora_msg.c
        storage.c
)


# Add the main executable
add_executable(${PROJECT_NAME}
        # main
        main.c
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