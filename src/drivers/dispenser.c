#include "pico/stdlib.h"
#include "dispenser.h"


void setup_dispenser(const dispenser_t *dispenser) {
    for (int i = 0; i < sizeof(dispenser->pins)/sizeof(dispenser->pins[0]); i++) {
        gpio_init(dispenser->pins[i]);
        gpio_set_dir(dispenser->pins[i], GPIO_OUT);
    }
    gpio_init(dispenser->opto_fork);
    gpio_set_dir(dispenser->opto_fork, GPIO_IN);
    gpio_pull_up(dispenser->opto_fork);
    gpio_init(dispenser->piezo);
    gpio_set_dir(dispenser->piezo, GPIO_IN);
    gpio_pull_up(dispenser->piezo);
}

void run_dispenser(dispenser_t *dispenser) {
    if (dispenser->direction == COUNTER_CLOCKWISE) {
        dispenser->step = (dispenser->step + 1) % 8;
    } else {
        dispenser->step = (dispenser->step - 1 + 8) % 8;
    }
    uint next_step = dispenser->step_bits[dispenser->step];
    for (int i = 0; i < sizeof(dispenser->pins)/sizeof(uint); i++) {
        gpio_put(dispenser->pins[i], (next_step >> i) & 1);
    }
    sleep_ms(2);
}