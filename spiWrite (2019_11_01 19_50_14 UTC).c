/*
 * File:   spiWrite.c
 * Author: timsp
 *
 * Created on 27 februari 2019, 16:55
 */


#include <xc.h>
#include "Init.h"
#include "Definess.h"

void spiWrite(int Getal)
{
    int i;
    for(i = 0; i < 5; i++)              //lees hele array waarde af om te sturen
    {
        SSPBUF = Letters[Getal][i];     //stuur bits
        while(!BF);                     //wacht to SPI klaar is
    }
}