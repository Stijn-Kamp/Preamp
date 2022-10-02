/*
 * File:   spiWriteInput.c
 * Author: timsp
 *
 * Created on 27 februari 2019, 16:57
 */


#include <xc.h>
#include "Init.h"
#include "Definess.h"

void spiWriteInput(int Getal)
{
    PORTDbits.RD4 = 0b0;    //zet latch laag
    
    //stuur aparte letters
    spiWrite(ledI);
    spiWrite(ledN);
    spiWrite(ledP);
    spiWrite(ledU);
    spiWrite(ledT);
    spiWrite(DUBBELEPUNT);
    spiWrite(SPATIE);
    spiWrite(Getal);        //schrijf het gewenste getal
    PORTDbits.RD4 = 0b1;    //zet hoog voor latch
}
