#include "pico/stdlib.h"
#include <stdio.h>

#define PIEZO_PIN 27

int main(void) {
    stdio_init_all();
    gpio_init(PIEZO_PIN);
    gpio_set_dir(PIEZO_PIN, GPIO_IN);
    gpio_pull_up(PIEZO_PIN);

    while (true) {
        if (gpio_get(PIEZO_PIN) == 0) {
            uint32_t time = to_ms_since_boot(get_absolute_time());
            printf("%d - Piezo!\n", time);
        }
    }
    return 0;
}