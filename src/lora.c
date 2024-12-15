#include "lora.h"
#include "lib/uart_irq/uart.h"
#include "lib/debug.h"
#include <pico/stdio.h>
#include <pico/time.h>
#include <stdio.h>
#include <string.h>

#define SIZE 256
#define send_time_limit 50000
#define connect_time_limit 200000

typedef struct {
    int baud_rate;
    int tx_pin;
    int rx_pin;
    int uart_nr;
    int event_code;
    char *message;
    char *app_key;
    uint times_limit;
    uint timeout_us;
} lora_t;

lora_t lora_basic_configuration() {
    lora_t lora;
    lora.baud_rate = 9600;
    lora.tx_pin = 4;
    lora.rx_pin = 5;
    lora.uart_nr = 1;
    lora.event_code = 0;
    lora.message = "";
    lora.app_key = "bb1afcb9f4525f435c00d82a03f5f483";
    lora.times_limit = 5;
    lora.timeout_us = 500000;
    return lora;
}

void connect_lora(const lora_t *lora) {
#if ENABLE_LORA
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
        const uint first_time = to_ms_since_boot(get_absolute_time());
        while (i == 4 && strstr(str[i], "+JOIN: Network joined") == NULL &&
               to_ms_since_boot(get_absolute_time()) - first_time < connect_time_limit) {
            if (strstr(str[i], "+JOIN: Join failed") != NULL) {
                uart_send(lora->uart_nr, message[i]);
            }
            uart_read(lora->uart_nr, str[i], sizeof(str));
            sleep_ms(1000);
            printf("linking ...\n");
        }
        if (i == 4 && strstr(str[i], "+JOIN: Network joined") == NULL) {
            printf("connect failed\n");
            return;
        }
    }
    dprintf(DEBUG_LEVEL_INFO, "network connect successfully\n");
#else
    dprintf(DEBUG_LEVEL_DEBUG, "connect_lora (lora is disabled)\n");
#endif
}

int send_message(const int event_code, char *message) {
#if ENABLE_LORA
    char message_buf[SIZE];
    snprintf(message_buf, SIZE, "AT+MSG=\"[%d]%s\"\r\n", event_code, message);
    uart_send(1, message_buf);
    char str[SIZE] = {0};
    uart_read(1, str, sizeof(str));
    const uint first_time = to_ms_since_boot(get_absolute_time());
    while (strstr(str, "+MSG: Done") == NULL && to_ms_since_boot(get_absolute_time()) - first_time < send_time_limit) {
        uart_send(1, "");
        sleep_ms(300);
        uart_read(1, str, sizeof(str));
    }
    if (strstr(str, "+MSG: Done") == NULL) {
        printf("send failed\n");
        return 0;
    }
    dprintf(DEBUG_LEVEL_INFO, "%s send successfully\n", message);
    return 1;
#else
    dprintf(DEBUG_LEVEL_DEBUG, "send_message (lora is disabled)\n");
    return 1;
#endif
}

void lora_init_and_connect() {
#if ENABLE_LORA
    const lora_t lora = lora_basic_configuration();
    uart_setup(lora.uart_nr, lora.tx_pin, lora.rx_pin, lora.baud_rate);
    connect_lora(&lora);
#else
    dprintf(DEBUG_LEVEL_DEBUG, "lora_init_and_connect (lora is disabled)\n");
#endif
}
