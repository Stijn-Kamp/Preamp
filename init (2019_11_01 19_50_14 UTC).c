/*
 * File:   Init.c
 * Author: timsp
 *
 * Created on 27 februari 2019, 16:09
 */


#include <xc.h>
#include "Init.h"
#include "Definess.h"

void Init(void)
{
    
    /*Clock*/
    OSCCONbits.IRCF = 0b110;                                                     //4MHz clock speed
    OSCCONbits.OSTS = 0b1;
    OSCCONbits.HTS = 0b0;
    OSCCONbits.LTS = 0b0;
    OSCCONbits.SCS = 0b0;

    /*Invoer/uitvoer select*/
    /*Leds*/
    TRISAbits.TRISA0 = 0b0;
    TRISAbits.TRISA1 = 0b0;
    TRISAbits.TRISA2 = 0b0;
    TRISAbits.TRISA3 = 0b0;
    /*Rotory encoder*/
    TRISBbits.TRISB4 = 0b1;
    TRISBbits.TRISB5 = 0b1;
    
    TRISEbits.TRISE2 = 0b1;                                                     //de RE2 poort is input
    
    //config for screen 1(input))
    TRISDbits.TRISD2 = 0b0; //configure port as output D2 is reset
    TRISDbits.TRISD4 = 0b0; //configure port as output D4 is CE
    TRISDbits.TRISD6 = 0b0; //configure port as output D6 is blank
    TRISDbits.TRISD7 = 0b0; //configure port as output D7 is register select
    
    //config for screen 2(vol)
    TRISCbits.TRISC2 = 0b0; //configure port as output C2 is reset
    TRISCbits.TRISC3 = 0b0; //clear this bit for SCK
    TRISCbits.TRISC4 = 0b0; //configure port as output C4 is CE
    TRISCbits.TRISC5 = 0b0; //clear this bit for SDO
    TRISCbits.TRISC6 = 0b0; //configure port as output C6 is blank
    TRISCbits.TRISC7 = 0b0; //configure port as output D7 is register select

    /*ADC*/
    ADCON0bits.ADCS = 0b01;                                                     //FOSC/8
    ADCON0bits.CHS = 0b0111;                                                    //Analog channel selecteer AN7
    ADCON0bits.ADON = 0b1;                                                      //Enable de ADC
    ADCON1bits.ADFM = 0b1;                                                      //ADC is right justified
    ADCON1bits.VCFG1 = 0b0;                                                     //geen reference port
    
    /*Analoog select*/
    /*Leds*/
    ANSELbits.ANS0 = 0b0;
    ANSELbits.ANS1 = 0b0;
    ANSELbits.ANS2 = 0b0;
    ANSELbits.ANS3 = 0b0;
    /*ADC*/
    ANSELbits.ANS7  =   0b1;                                                    //de ADCport is analoog
    /*Rotory encoder*/
    ANSELHbits.ANS11 = 0b0;
    ANSELHbits.ANS13 = 0b0;
   
    /*Interrupt bits*/
    PORTBbits.RB4 = 0b1;
    PORTBbits.RB5 = 0b1;
    INTCONbits.RBIE = 0b1;                                                      /*RB interrupt on change enable*/
    INTCONbits.RBIF = 0b0;                                                      /*RB interrupt flag*/
    IOCBbits.IOCB4 = 0b1;                                                       /*RB4 interrupt enable*/
    IOCBbits.IOCB5 = 0b1;                                                       /*RB5 interrupt enable*/
    
    //instellingen voor de afstandbediening
    PORTBbits.RB0 = 1;                                                          //receiver is input
    
    ANSELHbits.ANS12 = 0;                                                       //receiver is digital
    
    TRISCbits.TRISC0 = 0;                                                       //Ra0 is output (motor)
    TRISCbits.TRISC1 = 0;                                                       //Ra1 is output (motor)
    TRISDbits.TRISD0 = 0; 
    
    OPTION_REGbits.INTEDG = 0;                                                  //Interrupts zijn falling edge 
    
    INTCONbits.INTE = 1;                                                        //ON: interrupts on change
    INTCONbits.INTF = 0;                                                        //interrupt bit wordt hoog wamnneer dr een interrupt is.
    
    PORTCbits.RC1 = 0;                                                          //zet beiden kanten van de motor uit.
    PORTCbits.RC0 = 0;              
    PORTDbits.RD0 = 0;

        
    PORTA = 0b00001110;                                                         /*Start met alleen RA0 laag (Audioingang 0 hoog)*/
    
    /*fix de schermpjes*/
    spiInit();                                                                  //initialiseer SPI
    
    spiWord(0b01111101);                                                                   //stuur control word0    0b01000101
    spiWord(0b10000001);                                                                 //stuur control word1
    
    Teller = eeprom_read(INPUTAD);                                              //als er een waarde staat opgeslagen in eeprom
    Kanaal = (unsigned int)(((Teller) % (KANALENSCHAAL))/SCHAAL);
    PORTA = 0b1111 - (1 << Kanaal);                                             /*Zet de juiste led aan*/
    Kalibreren();
    PIE1bits.TMR2IE = 1;
    INTCONbits.GIE = 0b1;                                                    /*Global interrupt enable*/
}

