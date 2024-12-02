#ifndef LED_H
#define LED_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t pin;
    bool state;
} led_t;

void led_init(led_t *led);
void set_led(led_t *led, bool state);

#endif // LED_H
