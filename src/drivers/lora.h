#ifndef LORA_H
#define LORA_H

#include "hardware/uart.h"

typedef struct {
    int baudrate;
    uint tx_pin;
    uint rx_pin;
    uart_inst_t *uart;
} lora_t;

void setup_lora(const lora_t *lora);

#endif // LORA_H
