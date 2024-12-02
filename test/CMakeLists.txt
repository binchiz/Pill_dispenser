include_directories(
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB TEST_SOURCES "*.c")

foreach (TEST_SOURCE ${TEST_SOURCES})
    # get filename without extension
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    # add executable
    add_executable(${TEST_NAME}
            ${TEST_SOURCE}
    )

    # link to the library
    target_link_libraries(${TEST_NAME}
            ${PROJECT_NAME}_lib
            pico_stdlib
            hardware_pwm
            hardware_gpio
            hardware_i2c
            hardware_uart
    )

    # output to UART
    pico_enable_stdio_usb(${TEST_NAME} 0)
    pico_enable_stdio_uart(${TEST_NAME} 1)

    # create map/bin/hex/uf2 files
    pico_add_extra_outputs(${TEST_NAME})
endforeach ()