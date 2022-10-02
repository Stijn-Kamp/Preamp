/*
 * File:   Kalibreren.c
 * Author: timsp
 *
 * Created on 27 februari 2019, 16:49
 */


#include <xc.h>
#include "Init.h"
#include "Definess.h"

void Kalibreren(void)
{
    MinSpanning = 1023;
    MaxSpanning = 0;
    int i;
    int ADres;                                                                  //om de 10bit adc waarde bij te houden
    char Controlle = eeprom_read(KALIBRATE);
    if(Controlle == CONTROLLECODE)                         //als er een waarde staat opgeslagen in eeprom
    {
        MinSpanning = eeprom_read(MINSPANNINGH) & 0b11;                         //dan wordt deze opgehaald
        MinSpanning <<= 8;
        MinSpanning += eeprom_read(MINSPANNINGL);
        MaxSpanning = eeprom_read(MAXSPANNINGH) & 0b11;                         //lees de MinSpanning van EEPROM
        MaxSpanning <<= 8;
        MaxSpanning += eeprom_read(MAXSPANNINGL);
    }
    else                                        
    {    
        PORTA = 0b1110;                                                         //Laat zien dat de potmeter naar links moet worden gedraaid 
        for(i =0 ; i <= KALIBREERPOGINGEN ;i++)                         
        {
            GO = 0b1;                                                           //zet de adc aan
            __delay_ms(KALIBREERTIJD);
            while( GO == 0b1 )continue;                                         //wacht tot deze klaar is
            ADres = ADRESH << 8 | ADRESL;                                       //sla de 10 bit adc waarde op
            if(ADres < MinSpanning)                                             //Als de adc lager is
            {
                MinSpanning = ADres;                                            //update MinSpanning           
            }
        }
    
        eeprom_write(MINSPANNINGL, MinSpanning);      //save in EEPROM
        eeprom_write(MINSPANNINGH, (MinSpanning >> 8) + (CONTROLLECODE << 2));  //save in EEPROM
        
        PORTA = 0b0111;                                                         //laat zien dat de potmeter naar rechts moet worden gedraaid
        for(i=0; i <= KALIBREERPOGINGEN; i++)
        {   
            ADCON0bits.GO = 0b1;                                                //zet de adc aan
            __delay_ms(KALIBREERTIJD);
            while( GO == 0b1 )continue;                                         //wacht tot de adc klaar is
            ADres = ADRESH << 8 | ADRESL;                                       //sla de 10 bit adc waarde op
            if(ADres > MaxSpanning)                                             //als de adc hoger is
            {
                MaxSpanning = ADres;                                            //update MinSpanning
            }
        }        
        eeprom_write(MAXSPANNINGL, MaxSpanning);      //save in EEPROM
        eeprom_write(MAXSPANNINGH, (MaxSpanning >> 8) + (CONTROLLECODE << 2));//save in EEPROM
    }
    eeprom_write(KALIBRATE, CONTROLLECODE);
}
