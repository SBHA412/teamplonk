#include "i2c.h"

volatile uint8_t receiveBuffer[BUFFER_SIZE];
uint8_t nextByte = 0;

void setupI2C() {
    // Configure I/O Pins
    P1SEL0 |= BIT2; // Select P1.2 -> SDA (Serial Data Line)
    P1SEL1 &= ~BIT2;

    P1SEL0 |= BIT3; // Select P1.3 -> SCL (Serial Clock Line)
    P1SEL1 &= ~BIT3;

    UCB0CTLW0 |= UCSWRST;           // Hold eUSCI in reset state
    UCB0CTLW0 |= UCMST | UCMODE_3;  // I2C Master Mode
    UCB0CTLW1 |= UCASTP_2;          // Automatic stop condition after UCB0TCNT bytes
                                    //    - Stop condition must be handled by software
                                    //      for dynamic packet sizes (REMOVE ONCE IMPLEMENTED)
        
    UCB0CTLW0 |= UCSSEL__SMCLK;     // BRCLK -> SMCLK = 16MHz
    UCB0BRW = 40;                   // I2C CLK -> BRCLK/40 = 400kHz

    UCB0CTLW0 &= ~UCSWRST;          // Initialise eUSCI module
}

// REMOVE ONCE SW STOP CONDITION IMPLEMENTED
void changePacketSize(uint8_t packetSize) {
    UCB0CTLW0 |= UCSWRST;           // Hold eUSCI in reset state
                                    //    - Necessary when changing packet size
    UCB0TBCNT = packetSize;  // Set packet size
    UCB0CTLW0 &= ~UCSWRST;          // Re-initialise eUSCI module
}
