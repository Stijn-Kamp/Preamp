/* 
 * File:   Definess.h
 * Author: timsp
 *
 * Created on 27 februari 2019, 16:30
 */

#ifndef DEFINESS_H
#define	DEFINESS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define AANTALKANALEN 4                                                         /*Aantal kanalen(LED's)*/
#define SCHAAL 5
#define CONTROLLES 1
#define MAXSPANNINGH 0                        //De 8 least significant bits van de maxspanning
#define MAXSPANNINGL 1                        //De 8 least significant bits van de maxspanning
#define MINSPANNINGH 2                        //De 8 least significant bits van de maxspanning
#define MINSPANNINGL 3                        //De 8 least significant bits van de maxspanning
#define INPUTAD      4                        //Input adres in eeprom
#define KALIBRATE    5

const char KANALENSCHAAL = SCHAAL * AANTALKANALEN;

#define INPUT1  0b10110111              //Defineer de verschillende INPUTS bit patroon
#define INPUT2  0b01111011
#define INPUT3  0b10101111
#define INPUT4  0b10111011

#define HARDER  0b01101111              //defineer verschillende volume INPUTS bit patroon
#define ZACHTER 0b01110111
    
#define RECEIVERCYCLE   4.16         //in ms dus 0,42 ms

#define CONTROLLECODE 0b101010                  //Hiermee wordt gecontrolleerd of de waardes in eeprom zijn opgeslagen

#define _XTAL_FREQ 4000000                      //Used by the XC8 delay_ms(x) macro
#define DEADZONE 15                              //helft van de deadzone
#define MAXVOL 30                                //max volume
#define TRUE 1                                  //True kan nu gebruikt worden

#define KALIBREERPOGINGEN 255                   //Aantal keren dat er gecontroleerd wordt
#define KALIBREERTIJD 15                        //Tijd in ms tussen de controlles

//defines die het makkelijker maken om letters naar de displays te sturen
#define ledV 10
#define ledO 11
#define ledL 12
#define ledI 13
#define ledN 14
#define ledP 15
#define ledU 16
#define ledT 17
#define PUNT 18
#define DUBBELEPUNT 19
#define SPATIE 20



const unsigned char Letters[21][5] = 
{
    {0x3E, 0x51, 0x49, 0x45, 0x3E},// 0
    {0x00, 0x42, 0x7F, 0x40, 0x00},// 1
    {0x62, 0x51, 0x49, 0x49, 0x46},// 2
    {0x22, 0x41, 0x49, 0x49, 0x36},// 3
    {0x18, 0x14, 0x12, 0x7F, 0x10},// 4
    {0x27, 0x45, 0x45, 0x45, 0x39},// 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30},// 6
    {0x01, 0x71, 0x09, 0x05, 0x03},// 7
    {0x36, 0x49, 0x49, 0x49, 0x36},// 8
    {0x06, 0x49, 0x49, 0x29, 0x1E},// 9
    {0x07, 0x18, 0x60, 0x18, 0x07},// V
    {0x3E, 0x41, 0x41, 0x41, 0x3E},// O
    {0x7F, 0x40, 0x40, 0x40, 0x40},// L
    {0x00, 0x41, 0x7F, 0x41, 0x00},// I
    {0x7F, 0x04, 0x08, 0x10, 0x7F},// N
    {0x7F, 0x09, 0x09, 0x09, 0x06},// P
    {0x3F, 0x40, 0x40, 0x40, 0x3F},// U
    {0x01, 0x01, 0x7F, 0x01, 0x01},// T
    {0x00, 0x30, 0x30, 0x00, 0x00},// .
    {0x00, 0x36, 0x36, 0x00, 0x00},// :
    {0x00, 0x00, 0x00, 0x00, 0x00} // (spatie)
};

volatile int Kanaal = 0;                                                                 /*Houdt het kanaal bij*/
volatile int Teller = 0;                                                                 /*Houdt bij hoevaal er aan de rotory is gedraaid*/                                         

volatile char SchermUpdate = 0b11;
volatile unsigned int MaxSpanning = 1023;                                                   //Max voltage van de pot meter (is set to 0 so the first read will be higher)
volatile unsigned int MinSpanning = 0;                                                //Min voltage van de pot meter (is set to 1023 so the first read will be lower)
volatile unsigned int Volume = 0;                                                        //houdt het volume bij




typedef enum Richting{                                                          /*Geeft de richting aan*/
    Links = 1,
    Rechts = 0
} richting;


#ifdef	__cplusplus
}
#endif

#endif	/* DEFINESS_H */

