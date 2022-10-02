/*
 * File:   UpdateVolume.c
 * Author: timsp
 *
 * Created on 27 februari 2019, 16:50
 */


#include <xc.h>
#include "Init.h"
#include "Definess.h"

void UpdateVolume(void)
{
        static int AdcOud = 0;
        __delay_us(5);                                                              //wait for ADC charging cap to settle
        ADCON0bits.GO = 1;                                                      //zet de adc aan
        while(ADCON0bits.GO) continue;                                          //wacht tot de adc klaar is
        unsigned int ADres = ADRESH << 8 | ADRESL;                              //sla de 10 bit adc waarde op
        if(AdcOud + DEADZONE <= ADres || AdcOud - DEADZONE >= ADres)              //controleer of een update nodig is
        {
            Volume = (int)(ADres * MAXVOL / (MaxSpanning - MinSpanning));       //bereken het volume
            SchermUpdate |= 1;
            AdcOud = ADres;
        }
}
