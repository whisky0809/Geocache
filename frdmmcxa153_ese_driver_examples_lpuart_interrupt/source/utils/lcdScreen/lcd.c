#include "lcd.h"
#include "../source/utils/comProtocols/Lpi2c/lpi2c0_controller_interrupt.h"
#include <stdint.h>

#define LCD_ADDR 0x27
#define ENABLE 0x04
#define BACKLIGHT 0x08

void delay_us(unsigned int us)
{
    for (volatile unsigned int i = 0; i < us * 10; i++);
}

static void i2c_send(uint8_t data)
{
    // Toggle sanity LED (D13 on P3_15)
    GPIO3->PTOR = (1 << 15);  // Toggle LED each time we send to I2C

    lpi2c0_wait_busy();

    LPI2C0->MTDR = (0x04 << 8) | (LCD_ADDR << 1);  // START + ADDR
    LPI2C0->MTDR = (0x00 << 8) | data;             // DATA
    LPI2C0->MTDR = (0x02 << 8);                    // STOP
}

static void lcd_pulse_enable(uint8_t data)
{
    i2c_send(data | ENABLE | BACKLIGHT);
    delay_us(1);
    i2c_send((data & ~ENABLE) | BACKLIGHT);
    delay_us(50);
}

static void lcd_write4bits(uint8_t data)
{
    i2c_send(data | BACKLIGHT);
    lcd_pulse_enable(data);
}

static void lcd_send(uint8_t data, uint8_t mode)
{
    uint8_t high = data & 0xF0;
    uint8_t low  = (data << 4) & 0xF0;
    lcd_write4bits(high | mode);
    lcd_write4bits(low | mode);
}

void lcd_send_cmd(uint8_t cmd)
{
    lcd_send(cmd, 0x00);
}

void lcd_send_data(uint8_t data)
{
    lcd_send(data, 0x01);
}

void lcd_init(void)
{
    delay_us(50000);
    lcd_write4bits(0x30);
    delay_us(4500);
    lcd_write4bits(0x30);
    delay_us(150);
    lcd_write4bits(0x30);
    delay_us(150);
    lcd_write4bits(0x20);
    delay_us(150);

    lcd_send_cmd(0x28);
    lcd_send_cmd(0x0C);
    lcd_send_cmd(0x06);
    lcd_send_cmd(0x01);

    delay_us(2000);
}

void lcd_send_string(const char *str)
{
    while (*str)
    {
        lcd_send_data(*str++);
    }
}

void lcd_clear(void)
{
    lcd_send_cmd(0x01);
    delay_us(2000);
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
    uint8_t row_offsets[] = {0x00, 0x40};

    if (row > 1) row = 1;
    if (col > 15) col = 15;

    lcd_send_cmd(0x80 | (col + row_offsets[row]));
}
