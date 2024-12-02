#include "lora_msg.h"
#include "drivers/lora.h"
#include "hardware/uart.h"

static lora_t lora = {
    .baudrate = 9600,
    .tx_pin = 0,
    .rx_pin = 1,
    .uart = uart0,
};


