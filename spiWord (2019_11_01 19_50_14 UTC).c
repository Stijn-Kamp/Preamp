/*
 * File:   spiWord.c
 * Author: timsp
 *
 * Created on 27 februari 2019, 16:52
 */


#include <xc.h>
#include "Init.h"
#include "Definess.h"

void spiWord(char Word) //Write data to SPI bus
{
    PORTDbits.RD7 = 0b1;    //zet register select hoog
    PORTCbits.RC7 = 0b1;    //zet register select hoog
    PORTDbits.RD4 = 0b0;    //zet ce laag
    PORTCbits.RC4 = 0b0;    //zet ce laag
    SSPBUF = Word;          //stuur bits
    while(!BF);             //wacht tot de transmissie klaar is

    PORTDbits.RD4 = 0b1;    //zet ce hoog voor latch
    PORTCbits.RC4 = 0b1;    //zet ce hoog voor latch
    PORTCbits.RC7 = 0b0;    //zet register select laag
    PORTDbits.RD7 = 0b0;    //zet register select laag
}
