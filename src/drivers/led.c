#include "led.h"
#include "pico/stdlib.h"

void init_led(led_t *led) {
    gpio_init(led->pin);
    gpio_set_dir(led->pin, GPIO_OUT);
    set_led(led, false);
}

void set_led(led_t *led, bool state) {
    led->state = state;
    gpio_put(led->pin, state);
}

void toggle_led(led_t *led) {
    set_led(led, !led->state);
}