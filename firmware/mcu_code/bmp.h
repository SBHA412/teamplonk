#ifndef BMP_H
#define BMP_H

#include "i2c.h"


const Device bmp = {
    .addr = 0x76
};


/* BMP390 Base I2C Operations */

I2C_Mode BMP_readReg(uint8_t reg_addr, uint8_t *reg_data);

I2C_Mode BMP_burstReadReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count);

I2C_Mode BMP_writeReg(uint8_t reg_addr, uint8_t *reg_data);

I2C_Mode BMP_burstWriteReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count);


/* BMP390 Configuration */

uint8_t BMPdisable();

uint8_t BMP_enableForcedMode();

uint8_t BMP_flushFIFO();

uint8_t BMP_softReset();


/* BMP390 Sampling */

uint8_t BMP_getPressure(uint32_t *pressure);


#endif
