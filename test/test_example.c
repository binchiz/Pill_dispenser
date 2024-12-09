#include "drivers/led.h"
#include "pico/stdlib.h"
#include <stdio.h>

int main(void) {
    timer_hw->dbgpause = 0;
    stdio_init_all();
    printf("Hello from test_example!\n");

    init_led();

    while (true) {
        toggle_led();
        sleep_ms(500);
        printf("LED toggled\n");
    }
    return 0;
}