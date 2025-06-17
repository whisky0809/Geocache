#ifndef LPI2C0_CONTROLLER_INTERRUPT_H
#define LPI2C0_CONTROLLER_INTERRUPT_H

#include <MCXA153.h>
#include <stdbool.h>

void lpi2c0_controller_init(void);
void lpi2c0_wait_busy(void);
void lpi2c0_putword(const uint16_t data);
uint16_t lpi2c0_getword(void);
uint32_t lpi2c0_rxcnt(void);
uint32_t lpi2c0_txcnt(void);
bool lpi2c0_txfifo_full(void);
bool lpi2c0_rxfifo_empty(void);

#endif // LPI2C0_CONTROLLER_INTERRUPT_H
