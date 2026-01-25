#ifndef SPI_H
#define SPI_H

#include "msp430fr2355.h"
#include <stdint.h>

void setupSPI();
uint8_t spiTransferByte(uint8_t byte);

#endif
