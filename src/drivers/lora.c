#include "lora.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "uart_irq/uart.h"

#include <hardware/timer.h>
#include <stdio.h>
#include <string.h>
#include <pico/stdio.h>
#include <pico/time.h>

#define SIZE 256
#define send_time_limit 50000

lora_t lora_basic_configuration() {
    lora_t lora;
    lora.baud_rate = 9600;
    lora.tx_pin = 4;
    lora.rx_pin = 5;
    lora.uart_nr = 1;
    lora.event_code = 0;
    lora.message = "";
    lora.app_key = "1e89ed8348de1b42ce000d49f4d8c893";
    lora.times_limit = 5;
    lora.timeout_us = 500000;
    return lora;
}

void connect_lora(const lora_t *lora) {
    char message[5][SIZE] = {0};
    char str[5][SIZE] = {0};
    snprintf(message[0], SIZE, "AT+MODE=LWOTAA\r\n");
    snprintf(message[1], SIZE, "AT+KEY=APPKEY,\"%s\"\r\n", lora->app_key);
    snprintf(message[2], SIZE, "AT+CLASS=A\r\n");
    snprintf(message[3], SIZE, "AT+PORT=8\r\n");
    snprintf(message[4], SIZE, "AT+JOIN\r\n");

    for (int i = 0; i < 5; ++i) {
        memset(str[i], 0, sizeof(str[i]));
        uart_send(lora->uart_nr, message[i]);
        sleep_ms(200);
        uart_read(lora->uart_nr, str[i], sizeof(str));
        while (i == 4 && strstr(str[i], "+JOIN: Network joined") == NULL) {
            uart_read(lora->uart_nr, str[i], sizeof(str));
            sleep_ms(1000);
            printf("linking ...\n");
        }
    }
    printf("network connect successfully\n");
}

int send_message(const int event_code, char *message) {
    char message_buf[SIZE];
    snprintf(message_buf, SIZE, "AT+MSG=\"[%d]%s\"\r\n", event_code, message);
    uart_send(1, message_buf);
    char str[SIZE] = {0};
    uart_read(1, str, sizeof(str));
    uint first_time = to_ms_since_boot(get_absolute_time());
    while (strstr(str, "+MSG: Done") == NULL && to_ms_since_boot(get_absolute_time()) - first_time < send_time_limit) {
        uart_send(1, "");
        sleep_ms(300);
        uart_read(1, str, sizeof(str));
    }
    if (strstr(str, "+MSG: Done") == NULL) {
        printf("send failed\n");
        return 0;
    }
    printf("%s send successfully\n", message);
    return 1;
}


void lora_init_and_connect() {
    const lora_t lora = lora_basic_configuration();
    uart_setup(lora.uart_nr, lora.tx_pin, lora.rx_pin, lora.baud_rate);
    connect_lora(&lora);
}

// int main(void) {
//     stdio_init_all();
//     lora_init_and_connect();
//     send_message(BOOT, "sheng,bc,zzy!");
//     return 0;
// }
