#ifndef TCS34725_H
#define TCS34725_H

#include <stdint.h>
#include <stdbool.h>

// Sensor I2C Address
#define TCS34725_ADDR 0x29

// Register Definitions
#define TCS34725_STATUS  0x13
#define TCS34725_AVALID  0x01
#define TCS34725_ENABLE  0x00
#define TCS34725_ATIME   0x01
#define TCS34725_CONTROL 0x0F
#define TCS34725_CDATAL  0x14
#define TCS34725_ID      0x12
#define TCS34725_CDATA	 0x14         // Clear data low byte
#define TCS34725_RDATA   0x16         // Red data low byte
#define TCS34725_GDATA   0x18         // Green data low byte
#define TCS34725_BDATA   0x1A         // Blue data low byte

// Command Register Settings
#define TCS34725_CMD_BIT     0x80
#define TCS34725_AUTO_INC    0x20

typedef struct {
    uint16_t clear;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} color_data_t;

void tcs34725_init(void);
bool tcs34725_get_color_data(color_data_t *color_data);
bool tcs34725_data_available(void);

#endif // TCS34725_H
