#include "drivers/led.h"
#include "pico/stdlib.h"
#include <stdio.h>

int main(void) {
    stdio_init_all();
    printf("Hello from test_example!\n");

    led_t led = {.pin = 22, .state = false};
    init_led(&led);

    while (true) {
        toggle_led(&led);
        sleep_ms(500);
    }
    return 0;
}