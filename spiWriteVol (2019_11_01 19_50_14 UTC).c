/*
 * File:   spiWriteVol.c
 * Author: timsp
 *
 * Created on 27 februari 2019, 16:53
 */


#include <xc.h>
#include "Init.h"
#include "Definess.h"

void spiWriteVol(int Getal)
{    
    PORTCbits.RC4 = 0b0;    //zet register select laag
    //stuur aparte letters
    spiWrite(ledV);
    spiWrite(ledO);
    spiWrite(ledL);
    spiWrite(PUNT);
    spiWrite(SPATIE);
    spiWrite(SPATIE);
    char c = (Getal - Getal%10)/10;
    if(c == 0) c = SPATIE;
    spiWrite(c);    //bereken welk getal er als eerste op het scherm moet staan en stuur het
    spiWrite(Getal%10);                 //bereken welk getal er als eerste op het scherm moet staan en stuur het
    PORTCbits.RC4 = 0b1;                //zet hoog voor latch
}
