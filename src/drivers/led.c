#include "led.h"
#include "pico/stdlib.h"

static led_t led = {20, false};

void init_led() {
    gpio_init(led.pin);
    gpio_set_dir(led.pin, GPIO_OUT);
    set_led(false);
}

void set_led(bool state) {
    led.state = state;
    gpio_put(led.pin, state);
}

void toggle_led() {
    set_led(!led.state);
}

void toggle_led_n_times(int times) {
    for (int i = 0; i < times; i++) {
        set_led(false);
        sleep_ms(200);
        set_led(true);
        sleep_ms(200);
    }
}