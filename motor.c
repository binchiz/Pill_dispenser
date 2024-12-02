#include "pico/stdlib.h"
#include "motor.h"


void setup_motor(const motor_t *motor) {
    for (int i = 0; i < sizeof(motor->pins); i++) {
        gpio_init(motor->pins[i]);
        gpio_set_dir(motor->pins[i], GPIO_OUT);
    }
}

void run_motor(motor_t *motor) {
    if (motor->direction == COUNTER_CLOCKWISE) {
        motor->step = (motor->step + 1) % 8;
    } else {
        motor->step = (motor->step - 1 + 8) % 8;
    }
    uint next_step = motor->step_bits[motor->step];
    for (int i = 0; i < sizeof(motor->pins)/sizeof(uint); i++) {
        gpio_put(motor->pins[i], (next_step >> i) & 1);
    }
}