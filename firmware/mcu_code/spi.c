#include "spi.h"

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
    UCB1BRW = 80;                                   // SPI CLK -> BRCLK/80 = 200kHz
    
    UCB1CTLW0 &= ~UCSWRST;                          // Initialise eUSCI module
}

uint8_t spiTransferByte(uint8_t byte) {
    while (!(UCB1IFG & UCTXIFG));   // Wait for TX buffer
    UCB1TXBUF = byte;
    // MISO unused for SD storage operations
    while (!(UCB1IFG & UCRXIFG));   // Wait for RX buffer
    //return UCB1RXBUF;
    return 0;
}
