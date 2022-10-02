/*
 * File:   SpiInit.c
 * Author: timsp
 *
 * Created on 27 februari 2019, 16:41
 */


#include <xc.h>
#include "Definess.h"
#include "Init.h"
void spiInit(void)
{
    SSPCONbits.SSPM = 0b0000; // SPI Master mode, clock = FOSC/4
    SSPCONbits.CKP = 0b0; //idle state low       
    SSPSTATbits.CKE = 0b1; // Data transmitted 
    SSPSTATbits.SMP = 0b1; // Input data sampled at end of data output time
    
    SSPCONbits.SSPEN = 0b1; //turn on SPI
    
    PORTDbits.RD2 = 0b1;    //reset lijn moet altijd hoog zijn anders komt er een reset 
    PORTCbits.RC2 = 0b1;    //reset lijn moet altijd hoog zijn anders komt er een reset
    
    PORTCbits.RC6 = 0b0; //maak blank lijn standaard laag
    PORTDbits.RD6 = 0b0; //maak blank lijn standaard laag
}
