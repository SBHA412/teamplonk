#include "spi.h"

uint8_t array[4] = {0xAA, 0xBB, 0xCC, 0xDD}; // Implement method of queuing data
static uint8_t position = 0;

void setupSPI() {
    // Configure I/O Pins
    P4SEL0 |= BIT6; // Select P4.6 -> MOSI (Data) SENDING OUT DATA
    P4SEL1 &= ~BIT6;

    P4SEL0 |= BIT7; // Select P4.7 -> MISO (Data) GETTING DATA
    P4SEL1 &= ~BIT7;

    P4SEL0 |= BIT5; // Select P4.5 -> CLK (Clock)
    P4SEL1 &= ~BIT5;

    UCB1CTLW0 |= UCSWRST;                           // Hold eUSCI in reset state
    UCB1CTLW0 |= UCMSB | UCSYNC | UCMST | UCCKPL;   // MSB First, Synchronous, Master, Clock inactive state high

    UCB1CTLW0 |= UCSSEL__SMCLK;                     // BRCLK -> SMCLK = 16MHz
    UCB1BRW = 10;                                   // SPI CLK -> BRCLK/80 = 200kHz (Initialisation)
    
    UCB1CTLW0 &= ~UCSWRST;                          // Initialise eUSCI module
    UCB1IE |= UCTXIE;
    UCB1IFG &= ~UCTXIFG;
}

uint8_t spiTransferByte(uint8_t byte) {
    position = 0;
    UCB1TXBUF = array[position];
    
    return 0;
}

#pragma vector = EUSCI_B1_VECTOR
__interrupt void ISR_EUSCI_B1(void) {
    position++;
    if(position < sizeof(array)) {
        UCB1TXBUF = array[position];
    } else {
        UCB1IFG &= ~UCTXIFG;
    }
}
