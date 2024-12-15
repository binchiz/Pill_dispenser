#ifndef EEPROM_H
#define EEPROM_H

#include <hardware/i2c.h>
#include <stdbool.h>

/**
 * Note: do not assign i2c directly without initializing i2c instance.
 * Use init_eeprom to initialize it.
 */
typedef struct {
    i2c_inst_t *i2c;
    uint32_t baudrate;
    uint gpio_sda;
    uint gpio_scl;
    uint8_t device_addr;
    uint8_t page_size;
} eeprom_t;

void init_eeprom(eeprom_t *eeprom, i2c_inst_t *i2c);
int eeprom_write(const eeprom_t *eeprom, const uint8_t *data, uint16_t mem_addr, uint8_t len);
int eeprom_read(const eeprom_t *eeprom, uint8_t *buffer, uint16_t mem_addr, uint8_t len);
bool write_byte_with_checksum(const eeprom_t *eeprom, uint16_t mem_addr, uint16_t checksum_addr, uint8_t data);
bool read_byte_with_checksum(const eeprom_t *eeprom, uint16_t mem_addr, uint16_t checksum_addr, uint8_t *data);

#endif // EEPROM_H
