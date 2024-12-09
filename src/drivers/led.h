#ifndef LED_H
#define LED_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t pin;
    bool state;
} led_t;

void init_led(led_t *led);
void set_led(led_t *led, bool state);
void toggle_led(led_t *led);

#endif // LED_H
