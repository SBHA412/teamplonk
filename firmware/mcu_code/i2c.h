#ifndef I2C_H
#define I2C_H

#include "msp430fr2355.h"
#include <stdint.h>

#define MAX_BUFFER_SIZE     20


//******************************************************************************
// General I2C State Machine ***************************************************
//******************************************************************************

typedef enum I2C_ModeEnum{
    IDLE_MODE,
    NACK_MODE,
    TX_REG_ADDRESS_MODE,
    RX_REG_ADDRESS_MODE,
    TX_DATA_MODE,
    RX_DATA_MODE,
    SWITCH_TO_RX_MODE,
    SWITCH_TO_TX_MODE,
    TIMEOUT_MODE
} I2C_Mode;


/* Initialises the eUSCI_B0 module as I2C */
void I2C_init();

/* For slave device with dev_addr, read the data specified in slaves reg_addr.
 * The received data is copied into *reg_data (inefficient).
 *
 * --- dev_addr -> The slave device address.
 *           
 * --- reg_addr -> The register or command to send to the slave.
 *           
 * --- count ----> The length of data to read
 *           
 *  */
I2C_Mode I2C_ReadReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t count);

/* For slave device with dev_addr, writes the data specified in *reg_data
 * into the register at reg_addr.
 *
 * --- dev_addr --> The slave device address.
 *           
 * --- reg_addr --> The register or command to send to the slave.
 *           
 * --- *reg_data -> The buffer to write
 *           
 * --- count -----> The length of *reg_data
 *           
 *  */
I2C_Mode I2C_WriteReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t count);


#endif
