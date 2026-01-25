#ifndef I2C_H
#define I2C_H

#include "msp430fr2355.h"
#include <stdint.h>

#define BUFFER_SIZE 64  // Size of receive buffer in bytes

extern volatile uint8_t receiveBuffer[BUFFER_SIZE];

typedef struct i2cSlaveConfigParam {
    uint8_t slaveAddr;  // Address of active slave
    uint8_t packetSize; // Size of data packet in bytes
} i2cSlaveConfigParam;

void setupI2C();
void i2cSlaveConfig(uint8_t packetSize);
void i2cSendByte(uint8_t addr, uint8_t byte);
void i2cReceiveByte(uint8_t addr, uint8_t byte);

#endif
