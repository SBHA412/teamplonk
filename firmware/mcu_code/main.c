//******************************************************************************
//   MSP430FR235x Demo - eUSCI_B0, I2C Master multiple byte TX/RX
//
//   Description: I2C master communicates to I2C slave sending and receiving
//   3 different messages of different length. I2C master will enter LPM0 mode
//   while waiting for the messages to be sent/receiving using I2C interrupt.
//   ACLK = NA, MCLK = SMCLK = DCO 16MHz.
//
//                                     /|\ /|\
//                   MSP430FR2355      4.7k |
//                 -----------------    |  4.7k
//            /|\ |             P1.3|---+---|-- I2C Clock (UCB0SCL)
//             |  |                 |       |
//             ---|RST          P1.2|-------+-- I2C Data (UCB0SDA)
//                |                 |
//                |                 |
//                |                 |
//                |                 |
//                |                 |
//                |                 |
//
//   Xiaodong Li
//   Texas Instruments Inc.
//   May 2020
//   Built with CCS V9.2
//******************************************************************************

#include <msp430fr2355.h> 
#include <stdint.h>

#include "i2c.h"


//******************************************************************************
// Pin Config ******************************************************************
//******************************************************************************

#define LED_OUT    P2OUT
#define LED_DIR    P2DIR
#define LED_PIN    BIT3

//******************************************************************************
// I2C Config ******************************************************************
//******************************************************************************

#define SLAVE_ADDR      0x76

#define REG_ADDR        0x00

#define BYTE_COUNT      1

uint8_t receiveArray[BYTE_COUNT] = {0};
uint8_t transmitArray[BYTE_COUNT] = {15};


//******************************************************************************
// Device Initialization *******************************************************
//******************************************************************************


void GPIO_init() {
    LED_DIR |= LED_PIN;
    LED_OUT |= LED_PIN;
        
    // I2C pins
    P1SEL0 |= BIT2 | BIT3;
    P1SEL1 &= ~(BIT2 | BIT3);

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
}

void initClockTo16MHz() {
    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_1;

    // Clock System Setup
    __bis_SR_register(SCG0);                           // disable FLL
    CSCTL3 |= SELREF__REFOCLK;                         // Set REFO as FLL reference source
    CSCTL0 = 0;                                        // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);                            // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_5;                               // Set DCO = 16MHz
    CSCTL2 = FLLD_0 + 487;                             // DCOCLKDIV = 16MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                           // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));         // FLL locked
}


//******************************************************************************
// Main ************************************************************************
// Send and receive three messages containing the example commands *************
//******************************************************************************

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    initClockTo16MHz();
    GPIO_init();
    I2C_init();

    I2C_readReg(SLAVE_ADDR, REG_ADDR, receiveArray, BYTE_COUNT);

    __bis_SR_register(LPM0_bits + GIE); // LPM0 disables CPU, delete for normal operation
	
    while(1);
}
