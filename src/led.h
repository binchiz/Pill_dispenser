#ifndef LED_H
#define LED_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t pin;
    bool state;
} led_t;

void init_led();
void set_led(bool state);
void toggle_led();
void toggle_led_n_times(int times);

#endif // LED_H
