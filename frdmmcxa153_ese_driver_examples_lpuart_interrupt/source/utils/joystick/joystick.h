/*!
 * \file      joystick.h
 * \brief     Joystick interface (ADC + SW)
 * \author    len
 * \date      03/06/2025
 */

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>
#include <stdbool.h>

void joystick_init(void);
uint16_t joystick_vrx(void);
uint16_t joystick_vry(void);
bool joystick_sw(void);

#endif // JOYSTICK_H
