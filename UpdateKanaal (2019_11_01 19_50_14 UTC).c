/*
 * File:   UpdateKanaal.c
 * Author: timsp
 *
 * Created on 27 februari 2019, 16:39
 */


#include <xc.h>
#include "Definess.h"
#include "Init.h"

void UpdateKanalen(void)
{
    static int RB5oud;                                                          /*Houdt de oude waarde van RB5 bij*/
    static char Controlle = 0;                                                  /*Om het aantal controlles bij te houden*/
    richting Kant;
    __delay_ms(5);                                                              /*Debounce*/
    Kant = (PORTBbits.RB4 == PORTBbits.RB5);                                    /*Controleer welke kant de potmeter is opgedraaid*/
    Kant = (Kant == (PORTBbits.RB5 == RB5oud));                                 /*Iverteer de richting wanneer RB5 is verranderd aangezien deze is gespiegeld */
    RB5oud = PORTBbits.RB5;                                                     /*update RB5oud*/
    if((Controlle & 1) == Kant)                                                  /*controleer of de juiste richting op wordt gedraaid*/
    {
        Controlle += 2;                                                         /*Verhoog de controlle*/
    }
    else
    {
        Controlle = Kant;                                                       /*Hier wordt de controlle gereset*/
    }
    
    if((Controlle >> 1) >= (CONTROLLES))                                        /*Verhoogt alleen de richting bij genoeg controlles*/
    {
        if(Kant == Rechts)                                                      /*afhankelijk van de kant wordt de teller verhoogd of verlaagd*/
        {
            Teller++;
        }
        else
        {
            if(Teller == 0)                                                     /*Zet de teller op de max waarde wanneer hij overflowt*/
            {
                Teller = KANALENSCHAAL - 1;
            }
            else
            {
                Teller--;
            }
        }
        Controlle = Kant;
    }
    
    Kanaal = (unsigned int)(((Teller) % (KANALENSCHAAL))/SCHAAL);      /*Zet de teller om naar een van de kanalen*/
    PORTA = 0b1111 - (1 << Kanaal);                                             /*Zet de juiste led aan*/
    SchermUpdate |= 2;                                                          
    eeprom_write(INPUTAD, Teller);                                              //schrijf de teller naar het eeprom
}
