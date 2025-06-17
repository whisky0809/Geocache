/*! ***************************************************************************
 *
 * \brief     TCS34725 - Color Light-to-Digital Converter with IR Filter
 * \file      tcs34725_polling.c
 * \author    Hugo Arends
 * \date      February 2024
 *
 * \see       ams. (2013). TCS34725, TCS34727 Color Light-to-Digital Converter
 *            with IR Filter. Datasheet. DS000135_6-00. From:
 *            https://ams.com/documents/20143/36005/TCS3472_DS000135_6-00.pdf/
 *
 * \copyright 2024 HAN University of Applied Sciences. All Rights Reserved.
 *            \n\n
 *            Permission is hereby granted, free of charge, to any person
 *            obtaining a copy of this software and associated documentation
 *            files (the "Software"), to deal in the Software without
 *            restriction, including without limitation the rights to use,
 *            copy, modify, merge, publish, distribute, sublicense, and/or sell
 *            copies of the Software, and to permit persons to whom the
 *            Software is furnished to do so, subject to the following
 *            conditions:
 *            \n\n
 *            The above copyright notice and this permission notice shall be
 *            included in all copies or substantial portions of the Software.
 *            \n\n
 *            THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *            EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *            OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *            NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *            HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *            WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *            FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *            OTHER DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/
#include "tcs34725_polling.h"
#include "../comProtocols/Lpi2c/lpi2c0_controller_interrupt.h"

// -----------------------------------------------------------------------------
// Local function prototypes
// -----------------------------------------------------------------------------
static void delay(uint32_t count);
static bool tcs34725_write_register(uint8_t reg, uint8_t value);
static bool tcs34725_read_register(uint8_t reg, uint8_t *value);
static bool tcs34725_read_registers(uint8_t reg, uint8_t *data, uint8_t length);

// -----------------------------------------------------------------------------
// Local function implementation
// -----------------------------------------------------------------------------

static void delay(uint32_t count)
{
    for(volatile uint32_t i = 0; i < count; i++);
}

static bool tcs34725_write_register(uint8_t reg, uint8_t value)
{
    // Wait for bus to be free
    lpi2c0_wait_busy();

    // Clear status flags
    LPI2C0->MSR = LPI2C_MSR_STF_MASK | LPI2C_MSR_DMF_MASK |
                  LPI2C_MSR_PLTF_MASK| LPI2C_MSR_FEF_MASK |
                  LPI2C_MSR_ALF_MASK | LPI2C_MSR_NDF_MASK |
                  LPI2C_MSR_SDF_MASK | LPI2C_MSR_EPF_MASK;

    // Start + Write address
    while(lpi2c0_txfifo_full());
    LPI2C0->MTDR = LPI2C_MTDR_CMD(0b100) | LPI2C_MTDR_DATA(TCS34725_ADDR << 1);

    // Send register address with command bit
    while(lpi2c0_txfifo_full());
    LPI2C0->MTDR = LPI2C_MTDR_CMD(0b000) | LPI2C_MTDR_DATA(TCS34725_CMD_BIT | reg);

    // Send data
    while(lpi2c0_txfifo_full());
    LPI2C0->MTDR = LPI2C_MTDR_CMD(0b000) | LPI2C_MTDR_DATA(value);

    // Stop condition
    while(lpi2c0_txfifo_full());
    LPI2C0->MTDR = LPI2C_MTDR_CMD(0b010);

    // Wait for transaction to complete
    lpi2c0_wait_busy();


    // Check for errors
    if (LPI2C0->MSR & (LPI2C_MSR_ALF_MASK | LPI2C_MSR_NDF_MASK)) {
        return false;
    }

    return true;
}

static bool tcs34725_read_register(uint8_t reg, uint8_t *value)
{
    // 1) Wait for bus to be free
    lpi2c0_wait_busy();

    // 2) Clear flags before starting
    LPI2C0->MSR =  LPI2C_MSR_STF_MASK   |  // sticky STOP flag
                   LPI2C_MSR_DMF_MASK   |  // data match flag
                   LPI2C_MSR_PLTF_MASK  |  // pin‐low timeout flag
                   LPI2C_MSR_FEF_MASK   |  // FIFO error
                   LPI2C_MSR_ALF_MASK   |  // arbitration loss
                   LPI2C_MSR_NDF_MASK   |  // NACK detect
                   LPI2C_MSR_SDF_MASK   |  // STOP detect
                   LPI2C_MSR_EPF_MASK;     // end packet flag

    // 3) Build the “command byte” = 0x80 | (reg & 0x1F)
    uint8_t cmd = 0x80 | (reg & 0x1F);

    // 4) Enable the master (MEN = 1).
    LPI2C0->MCR |= LPI2C_MCR_MEN(1);

    // 5) Now push commands into MTDR in this order:
    //    (a) Start + SLA+W
    //    (b) Transmit “cmd” (command byte, e.g. 0x92 for reg=0x12)
    //    (c) Repeated-Start + SLA+R
    //    (d) Read 1 byte + STOP

    // Start + Write address (0x29<<1 | 0)
    while (lpi2c0_txfifo_full()) {}
    // CMD=0b100 means “Generate Start, then write DATA[7:0]” (DATA = SLA<<1 | 0)
    LPI2C0->MTDR = LPI2C_MTDR_CMD(0b100)  |
                   LPI2C_MTDR_DATA((TCS34725_ADDR << 1) | 0);

    //Transmit the command byte (0x80|reg)
    while (lpi2c0_txfifo_full()) {}
    //CMD=0b000 means “Transmit DATA[7:0]” (i.e. send the command byte)
    LPI2C0->MTDR = LPI2C_MTDR_CMD(0b000) |
                   LPI2C_MTDR_DATA(cmd);

    //Repeated-Start + Read address (0x29<<1 | 1)
    while (lpi2c0_txfifo_full()) {}
    // CMD=0b100 again, but DATA = SLA<<1 | 1 indicates “Generate Repeated-Start, then send SLA+R”
    LPI2C0->MTDR = LPI2C_MTDR_CMD(0b100)  |
                   LPI2C_MTDR_DATA((TCS34725_ADDR << 1) | 1);

    //Read one byte
    while (lpi2c0_txfifo_full()) {}
    // CMD=0b001 means “Receive (DATA[7:0]+1) bytes”
    LPI2C0->MTDR = LPI2C_MTDR_CMD(0b001) |
                   LPI2C_MTDR_DATA(0);

    // Command: 010b - Generate Stop condition on I2C bus
    // Data   : n.a.
    LPI2C0->MTDR = LPI2C_MTDR_CMD(0b010);


    // Wait for that 1 byte to actually arrive
    while (lpi2c0_rxfifo_empty()) {}

    // Read the received byte from MRDR
    *value = (uint8_t)LPI2C0->MRDR;



    // Check for NACK or arbitration loss
    if (LPI2C0->MSR & (LPI2C_MSR_ALF_MASK | LPI2C_MSR_NDF_MASK)) {
        return false;
    }

    return true;
}

static bool tcs34725_read_registers(uint8_t reg, uint8_t *data, uint8_t length)
{
    // Wait for bus to be free
    lpi2c0_wait_busy();

    // Clear status flags
    LPI2C0->MSR = LPI2C_MSR_STF_MASK | LPI2C_MSR_DMF_MASK |
                  LPI2C_MSR_PLTF_MASK | LPI2C_MSR_FEF_MASK |
                  LPI2C_MSR_ALF_MASK | LPI2C_MSR_NDF_MASK |
                  LPI2C_MSR_SDF_MASK | LPI2C_MSR_EPF_MASK;

    // Start + Write address
    while(lpi2c0_txfifo_full());
    LPI2C0->MTDR = LPI2C_MTDR_CMD(0b100) | LPI2C_MTDR_DATA(TCS34725_ADDR << 1);

    // Send register address with command bit and auto-increment
    while(lpi2c0_txfifo_full());
    LPI2C0->MTDR = LPI2C_MTDR_CMD(0b000) |
                   LPI2C_MTDR_DATA(TCS34725_CMD_BIT | TCS34725_AUTO_INC | reg);

    // Repeated start + Read address
    while(lpi2c0_txfifo_full());
    LPI2C0->MTDR = LPI2C_MTDR_CMD(0b100) | LPI2C_MTDR_DATA((TCS34725_ADDR << 1) | 1);

    // Read multiple bytes
    while(lpi2c0_txfifo_full());
    LPI2C0->MTDR = LPI2C_MTDR_CMD(0b001) | LPI2C_MTDR_DATA(length - 1);

    // Read all data bytes
    for(int i = 0; i < length; i++) {
        while(lpi2c0_rxfifo_empty());
        data[i] = (uint8_t)LPI2C0->MRDR;
    }


    // Command: 010b - Generate Stop condition on I2C bus
    LPI2C0->MTDR = LPI2C_MTDR_CMD(0b010);

    // Wait for transaction to complete
    lpi2c0_wait_busy();


    // Check for errors
    if (LPI2C0->MSR & (LPI2C_MSR_ALF_MASK | LPI2C_MSR_NDF_MASK)) {
        return false;
    }

    return true;
}

void tcs34725_init(void)
{
    uint8_t id;

    // Initialize I2C controller
    //lpi2c0_controller_init();

    // Add some delay after I2C initialization
    delay(120000);


    // Read ID register (should be 0x4D for TCS34727 (yes, all the functions are named wrong))
    if (!tcs34725_read_register(TCS34725_ID, &id)) {
        //verify ID is as expected
    }

    // Verify sensor ID
    if(id != 0x4D) {
        //Throw error if module not found
    }

    // Power on the sensor (PON=1)
    if (!tcs34725_write_register(TCS34725_ENABLE, 0x01)) {
        while(1); // Lock on I2C failure
    }

    // Wait for power-on
    delay(120000);



    // Enable ADC (PON=1, AEN=1)
    if (!tcs34725_write_register(TCS34725_ENABLE, 0x03)) {
        while(1); // Lock on I2C failure
    }

    // Set integration time to 50.4ms (0xEB = 235, time = (256-235)*2.4ms = 50.4ms)
    if (!tcs34725_write_register(TCS34725_ATIME, 0xEB)) {
        while(1); // Lock on I2C failure
    }

    // Add gain setting (4x gain)
    if (!tcs34725_write_register(TCS34725_CONTROL, 0x01)) {  // 0x01 = 4x gain
        while(1); // Lock on I2C failure
    }

    // Wait for first integration cycle to complete
    delay(500000);
}


bool tcs34725_data_available(void)
{
    uint8_t status;
    if (!tcs34725_read_register(TCS34725_STATUS, &status)) {
        return false;
    }
    return (status & TCS34725_AVALID);
}





bool tcs34725_get_color_data(color_data_t *color_data)
{
    LPI2C0->MCFGR1 &= ~(LPI2C_MCFGR1_AUTOSTOP(1));
	uint32_t m = __get_PRIMASK();
	__disable_irq();

	// critical section goes here ...


	uint8_t data[8];

    // Read all color data registers at once (8 bytes starting from CDATAL)
    if (!tcs34725_read_registers(TCS34725_CDATAL, data, 8)) {
        __set_PRIMASK(m);

        return false;
    }

    // Combine bytes (little-endian: LSB first)
    color_data->clear = data[0] | (data[1] << 8);
    color_data->red   = data[2] | (data[3] << 8);
    color_data->green = data[4] | (data[5] << 8);
    color_data->blue  = data[6] | (data[7] << 8);

    __set_PRIMASK(m);

    return true;

    // Critical section end
}
