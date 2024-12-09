#ifndef BUTTON_H
#define BUTTON_H
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    EVENT_NONE = 0,
    EVENT_SW0_PRESSED,
    EVENT_SW1_PRESSED,
    EVENT_SW2_PRESSED,
} event_t;

/**
 * \Breif Initialize the buttons, set up the handlers. But interrupts are disabled by default.
 */
void init_buttons();

/**
 * \Breif Enable the interrupts for the buttons.
 */
void enable_buttons();

/**
 * \Breif Disable the interrupts for the buttons.
 */
void disable_buttons();

/**
 * \Breif Get the button event from the queue.
 * \return The event type.
 */
event_t get_button_event();

#endif // BUTTON_H
