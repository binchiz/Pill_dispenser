#include "lora.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

void setup_lora(const lora_t *lora) {
    uart_init(lora->uart, lora->baudrate);
    gpio_set_function(lora->tx_pin, GPIO_FUNC_UART);
    gpio_set_function(lora->tx_pin, GPIO_FUNC_UART);
}
