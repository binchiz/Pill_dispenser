#ifndef LORA_MSG_H
#define LORA_MSG_H

#include <stdbool.h>
#include <stddef.h>

// todo define the LoRa functions, the below are just examples

/**
 * \brief Initialize the LoRa module
 * \Return Whether the LoRa module is initialized successfully
 */
bool connect_lora_wan();

/**
 * \brief Send a LoRa message to server
 */
void send_lora_msg(const char *msg, size_t len);

#endif // LORA_MSG_H
