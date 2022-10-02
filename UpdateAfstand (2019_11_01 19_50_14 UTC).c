/*
 * File:   UpdateAfstand.c
 * Author: timsp
 *
 * Created on 27 februari 2019, 16:58
 */


#include <xc.h>
#include "Init.h"
#include "Definess.h"

void UpdateAfstand(void)
{
        int i;                  //decladeren van varibalen.
        int Opslag1 = 0;
        int Opslag2 = 0;
        int GlobalInput;
        int Opdracht = 0;
        
        
        __delay_us(RECEIVERCYCLE);      //wacht 0,42 ms voor volgend bit
        
        if(PORTBbits.RB0 == 0)          //Als de Receiver bit laag is
        {
            
          
            
            
            
            for(i = 0; i < 8; i++)
            {   
                 __delay_us(RECEIVERCYCLE);         //wacht voor 0,42ms zodat volgend bit gelezen kan worden
                 Opslag1 <<= 1;                     //Shift Opslag 1 1 keer naar links
                 Opslag1 = PORTBbits.RB0;           //zet de receiver bit in Opslag1
                          
            }
            
            __delay_us((3*RECEIVERCYCLE));          //wacht 3 keer 0,42 ms zodat volgend bit gelezen kan worden
            __delay_us((80*RECEIVERCYCLE));         //wacht 80 keer 0,42 ms zodat volgend bit gelezen kan worden
            
            for(i = 0; i < 8; i++)
            {
                 __delay_us(RECEIVERCYCLE);         //wacht voor 0,42ms voor volgend bit
                 Opslag2 <<= 1;                     //Shift Opslag 2 1 keer naar links 
                 Opslag2 = PORTBbits.RB0;           //zet de receiver bit in Opslag2
            }
            
            if((Opslag1 << 7) == 0)                 //als deze bit 0 is betekent het dat het een continous signaal is en dus drie opties kan zijn
                                                    //Input 2, Harder of Zachter.
            {
                
                if (Opslag1 == INPUT2 || Opslag2 == INPUT2)
                {
                    PORTAbits.RA1 = 1;              //Als het bitpatroon overeen komt met input 2
                    GlobalInput = 1;                //zet de opdracht als INPUT 2 en de globalinput op 1 zodat deze naar dit kanaal springt
                    Opdracht = INPUT2;              
                }
                else if (Opslag1 == ZACHTER || Opslag2 == ZACHTER)
                {
                    
                    PORTCbits.RC1 = 0;              //Als De opslag overeenkomt met het zachter bitpatroon zet de motor aan naar links
                    PORTCbits.RC0 = 1;              //zodat het volume zachter wordt.
                    
                    Opdracht = ZACHTER;             //zet de opdracht op zachter
                }
                else if (Opslag1 == HARDER || Opslag2 == HARDER)
                {
                   
                    PORTCbits.RC0 = 0;              //Als De opslag overeenkomt met het harder bitpatroon zet de motor aan naar rechts
                    PORTCbits.RC1 = 1;              //zodat het volume harder wordt.
                    Opdracht = HARDER;              //zet de opdracht op harder
                }
                
                while(Opslag1 == Opdracht || Opslag2 == Opdracht)
                {
                    __delay_us(208*RECEIVERCYCLE);  //Als de opslag nog hetzelfde als de opdracht dan moet de motor aan blijven staan en dus gewacht worden
                    __delay_us(3*RECEIVERCYCLE);
                    
                unsigned int j;    
                    for(j=0;j<8;j++)
                    {
                            __delay_us(RECEIVERCYCLE);          //wacht en zet de bits van de receiver in Opslag 1
                            Opslag1 <<= 1;
                            Opslag1 = PORTBbits.RB0;
                    }
                __delay_us(80 * RECEIVERCYCLE);                 //wacht weer tot het volgende continous signaal
                __delay_us(3 * RECEIVERCYCLE);                  //lees niet de sign bits (010) in want die zijn nutteloos nu
                
                for(j=0;j<8;j++)
                    {
                            __delay_us(RECEIVERCYCLE);      //wacht en zet de bits van de receiver in Opslag 2
                            Opslag2 <<= 1;
                            Opslag2 = PORTBbits.RB0;
                    }
                }
                PORTCbits.RC0 = 0;                                                 //zet de motor uit
                PORTCbits.RC1 = 0;          
            }
            else
            {
                if(Opslag1 == INPUT1 || Opslag2 == INPUT1)          //Als het bit patroon overeenkomt met INPUT 1
                {
                                      //zet de GLobalinput op 0 zodat deze op input 1 komt
                    GlobalInput = 0;
                }
                if(Opslag1 == INPUT3 || Opslag2 == INPUT3)//Als het bit patroon overeenkomt met INPUT 3
                {
                    
                    GlobalInput = 2;    //zet de GLobalinput op 2 zodat deze op input 3 komt
                }
                if(Opslag1 == INPUT4 || Opslag2 == INPUT4)//Als het bit patroon overeenkomt met INPUT 4
                {
                    
                    GlobalInput = 3;  //zet de GLobalinput op 3 zodat deze op input 4 komt
                }
            }
            
        }  
}
