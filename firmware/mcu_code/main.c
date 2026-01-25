//#include <driverlib.h>
#ifndef DRIVERLIB_H
    #include "msp430fr2355.h"
    #include <stdint.h>
#endif

#include "i2c.h"
#include "spi.h"

void initClockTo16MHz() {
    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_1;

    __bis_SR_register(SCG0);    // disable FLL
    CSCTL3 |= SELREF__REFOCLK;  // Set REFO as FLL reference source
    CSCTL0 = 0;                 // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);     // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_5;        // Set DCO = 16MHz
    CSCTL2 = FLLD_0 + 487;      // set to fDCOCLKDIV = (FLLN + 1)*(fFLLREFCLK/n)
                                //                   = (487 + 1)*(32.768 kHz/1)
                                //                   = 16 MHz

    __delay_cycles(3);
    __bic_SR_register(SCG0);                        // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));      // FLL locked
}

int main(void) {
    // // Stop watchdog timer
    // WDT_A_hold(WDT_A_BASE);

    initClockTo16MHz(); // Sets MCLK=SMCLK -> 16MHz
    setupSPI();
    
    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    while(1)
    {
        spiTransferByte(0x7F); // Test byte
    }
}
