/*

 *
 * Created on 27 februari 2019, 16:05
 */
//Groepje 9: Stijn Kamp, Gerrit van den Berg & Tim Dons.
//project Embedded Systems




#define _XTAL_FREQ 4000000     // Used by the XC8 delay_ms(x) macro

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (RC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming disabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "Init.h"
#include "Definess.h"

void __interrupt() isr(void);

void main(void) 
{
        Init();                                                                     

    
    while(TRUE)
    {
        UpdateVolume();                                                         
        if(SchermUpdate & 1)                                                    //Kijk of er een schermupdate nodig is voor het volume
        {
            SchermUpdate &= 2;          
            spiWriteVol(Volume);        //volume functie
        }
        if(SchermUpdate & 2)                                                    //kijk of er een schermupdate nodig is voor het kanaal
        {           
            SchermUpdate &= 1;
            spiWriteInput(Kanaal+1);                                            //kanaal functie
        }
    }
}

void __interrupt() isr(void)
{
    if(INTCONbits.RBIF)                                                         /*Controleer welke interrupt er is getriggered*/
    {
        UpdateKanalen();                                                        /*update het kanaal*/
        INTCONbits.RBIF = 0b0;                                                  /*Reset de RB interrupt flag*/
    }
    if(INTCONbits.INTF)                                                         //Als de interuptbit van de receiver hoog
    {        
        UpdateAfstand();                                                           //handel de functie UpdateAdstand af
        INTCONbits.INTF = 0;                                                    //Zet de interrupt bit LAAG
    }
}
