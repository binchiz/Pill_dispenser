#include "buttons.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "utils/debug.h"

#define BUTTON_EVENT_QUEUE_SIZE 5
#define SW0_PIN 9
#define SW1_PIN 8
#define SW2_PIN 7
#define DEBOUNCE_DELAY 50

static queue_t button_event_queue;

static void init_button(uint pin);
static void sw_handler(uint sw_pin, uint64_t *last_pressed_time, event_t event);
static void sw0_handler();
static void sw1_handler();
static void sw2_handler();

static uint64_t sw0_last_pressed_time = 0;
static uint64_t sw1_last_pressed_time = 0;
static uint64_t sw2_last_pressed_time = 0;

static void init_button(uint pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}

static void sw_handler(uint sw_pin, uint64_t *last_pressed_time, event_t event) {
    if (gpio_get_irq_event_mask(sw_pin) & GPIO_IRQ_EDGE_RISE) {
        gpio_acknowledge_irq(sw_pin, GPIO_IRQ_EDGE_RISE);
        if (time_us_64() - *last_pressed_time > DEBOUNCE_DELAY * 1000) {
            *last_pressed_time = time_us_64();
            queue_try_add(&button_event_queue, &event);
        }
    }
}

static void sw0_handler() {
    sw_handler(SW0_PIN, &sw0_last_pressed_time, EVENT_SW0_PRESSED);
}

static void sw1_handler() {
    sw_handler(SW1_PIN, &sw1_last_pressed_time, EVENT_SW1_PRESSED);
}

static void sw2_handler() {
    sw_handler(SW2_PIN, &sw2_last_pressed_time, EVENT_SW2_PRESSED);
}

void init_buttons() {
    // init queue
    queue_init(&button_event_queue, sizeof(event_t), BUTTON_EVENT_QUEUE_SIZE);
    // init buttons
    init_button(SW0_PIN);
    init_button(SW1_PIN);
    init_button(SW2_PIN);
    gpio_add_raw_irq_handler(SW0_PIN, sw0_handler);
    gpio_add_raw_irq_handler(SW1_PIN, sw1_handler);
    gpio_add_raw_irq_handler(SW2_PIN, sw2_handler);
    irq_set_enabled(IO_IRQ_BANK0, true);
}

void enable_buttons() {
    gpio_set_irq_enabled(SW0_PIN, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(SW1_PIN, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(SW2_PIN, GPIO_IRQ_EDGE_RISE, true);
}

void disable_buttons() {
    gpio_set_irq_enabled(SW0_PIN, GPIO_IRQ_EDGE_RISE, false);
    gpio_set_irq_enabled(SW1_PIN, GPIO_IRQ_EDGE_RISE, false);
    gpio_set_irq_enabled(SW2_PIN, GPIO_IRQ_EDGE_RISE, false);
    // ensure the queue is empty
    event_t event;
    while (queue_try_remove(&button_event_queue, &event)) {
        // do nothing
    }
}

event_t get_button_event() {
    event_t event;
    if (queue_try_remove(&button_event_queue, &event)) {
        return event;
    } else {
        return EVENT_NONE;
    }
}
