/*
 * fifo16.h
 *
 *  Created on: 6 Jun 2025
 *      Author: henri
 */

#ifndef UTILS_COMPROTOCOLS_FIFO16_H_
#define UTILS_COMPROTOCOLS_FIFO16_H_


#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint16_t *data;
    uint32_t head;
    uint32_t tail;
    uint32_t cnt;
    uint32_t size;
    uint32_t mask;
} fifo_t;

void f16_init(fifo_t *f, uint16_t *buffer, const uint32_t size);
bool f16_push(fifo_t *f, uint16_t data);
bool f16_pop(fifo_t *f, uint16_t *data);
uint32_t f16_cnt(fifo_t *f);


#endif /* UTILS_COMPROTOCOLS_FIFO16_H_ */

