#include "led.h"
#include "pico/stdlib.h"

void led_init(led_t *led) {
    gpio_init(led->pin);
    gpio_set_dir(led->pin, GPIO_OUT);
    set_led(led, false);
}

void set_led(led_t *led, bool state) {
    led->state = state;
    gpio_put(led->pin, state);
}