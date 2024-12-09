#include "drivers/led.h"
#include "pico/stdlib.h"
#include <stdio.h>

int main(void) {
    stdio_init_all();
    printf("Hello from test_example!\n");

    init_led();

    while (true) {
        toggle_led();
        sleep_ms(500);
    }
    return 0;
}