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
#define AANTALKANALEN 4                                                         /*Aantal kanalen(LED's)*/
#define SCHAAL 3
#define CONTROLLES 1
#define MAXSPANNINGH 0                        //De 8 least significant bits van de maxspanning
#define MAXSPANNINGL 1                        //De 8 least significant bits van de maxspanning
#define MINSPANNINGH 2                        //De 8 least significant bits van de maxspanning
#define MINSPANNINGL 3                        //De 8 least significant bits van de maxspanning
#define INPUTAD      4                        //Input adres in eeprom

#define INPUT1  0b10110111              //Defineer de verschillende INPUTS bit patroon
#define INPUT2  0b01111011
#define INPUT3  0b10101111
#define INPUT4  0b10111011

#define HARDER  0b01101111              //defineer verschillende volume INPUTS bit patroon
#define ZACHTER 0b01110111
    
#define RECEIVERCYCLE   0.42         //in ms dus 0,42 ms

#define CONTROLLECODE 0b101010                  //Hiermee wordt gecontrolleerd of de waardes in eeprom zijn opgeslagen

#define _XTAL_FREQ 4000000                      //Used by the XC8 delay_ms(x) macro
#define DEADZONE 18                              //helft van de deadzone
#define MAXVOL 26                                //max volume
#define TRUE 1                                  //True kan nu gebruikt worden

#define KALIBREERPOGINGEN 255                   //Aantal keren dat er gecontroleerd wordt
#define KALIBREERTIJD 15                        //Tijd in ms tussen de controlles

//defines die het makkelijker maken om letters naar de displays te sturen
#define V 10
#define O 11
#define L 12
#define I 13
#define N 14
#define P 15
#define U 16
#define T 17
#define PUNT 18
#define DUBBELEPUNT 19
#define SPATIE 20

//array die alle waarden onthoud die naar het beeldscherm gestuurd kunnen worden
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

    
typedef enum Richting{                                                          /*Geeft de richting aan*/
    Links = 0,
    Rechts = 1
} richting;

volatile int Kanaal = 0;                                                                 /*Houdt het kanaal bij*/
volatile int Teller = 0;                                                                 /*Houdt bij hoevaal er aan de rotory is gedraaid*/                                         
volatile int RB5oud;                                                                     /*Houdt de oude waarde van RB5 bij*/

volatile char SchermUpdate = 0b11;
volatile unsigned int MaxSpanning = 1023;                                                   //Max voltage van de pot meter (is set to 0 so the first read will be higher)
volatile unsigned int MinSpanning = 0;                                                //Min voltage van de pot meter (is set to 1023 so the first read will be lower)
volatile unsigned int Volume = 0;                                                        //houdt het volume bij

void UpdateKanalen(void);                                                       /*Update het kanaal(LED's) na een verandering*/
void __interrupt() isr(void);                                                   /*interrupt handler*/
void Init(void);                                                                //Initialisatie van de PIC
void Kalibreren(void);                                                          //Kalibreren van de potmeter
void UpdateVolume(void);                                                        //Updaten van het volume

void spiInit(void);                                                             //initialiseer SPI
void spiWord(char);                                                             //functie om config word te sturen naar de displays
void spiWriteInput(int Getal);                                                  //fuctie die aangeroepen wordt om de input naar het display te schrijven
void spiWriteVol(int Getal);                                                    //fuctie die aangeroepen wordt om het volume naar het display te schrijven
void spiWrite(int);    
void UpdateAfstand(void);//functie die het schrijven naar de displays regelt

void main(void)
{
    Init();                                                                     
    Kalibreren();
    
    while(TRUE)
    {
        UpdateVolume();                                                         
        if(SchermUpdate & 1)                                                    //Kijk of er een schermupdate nodig is voor het volume
        {
            SchermUpdate &= 2;          
            spiWriteVol(Volume);        //volume functie
        }
        else if(SchermUpdate & 2)                                               //kijk of er een schermupdate nodig is voor het kanaal
        {           
            SchermUpdate &= 1;
            spiWriteInput(Kanaal);                                              //kanaal functie
        }
    }
}

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
    
    spiWord(0b01000101);                                                                  //stuur control word0
    spiWord(0b10000001);                                                                 //stuur control word1
    
    Teller = eeprom_read(INPUTAD);                                              //als er een waarde staat opgeslagen in eeprom
    UpdateKanalen();
    INTCONbits.GIE = 0b1;                                                       /*Global interrupt enable*/
}

void spiInit(void)
{
    SSPCONbits.SSPM = 0b0000; // SPI Master mode, clock = FOSC/4
    SSPCONbits.CKP = 0b0; //idle state low       
    SSPSTATbits.CKE = 0b1; // Data transmitted 
    SSPSTATbits.SMP = 0b1; // Input data sampled at end of data output time
    
    SSPCONbits.SSPEN = 0b1; //turn on SPI
    
    PORTDbits.RD2 = 0b1;    //reset lijn moet altijd hoog zijn anders komt er een reset 
    PORTCbits.RC2 = 0b1;    //reset lijn moet altijd hoog zijn anders komt er een reset
    
    PORTCbits.RC6 = 0b0; //maak blank lijn standaard laag
    PORTDbits.RD6 = 0b0; //maak blank lijn standaard laag
}

void UpdateKanalen(void)
{
    char Controlle = 0;                                                         /*Om het aantal controlles bij te houden*/
    richting Kant;
    __delay_ms(6);                                                              /*Debounce*/
    if(PORTBbits.RB4 == PORTBbits.RB5)                                          /*Controleer welke kant de potmeter is opgedraaid*/
    {
        Kant = Links;
    }
    else
    {
        Kant = Rechts;
    }
    Kant = (Kant == (PORTBbits.RB5 == RB5oud));                                 /*Iverteer de richting wanneer RB5 is verranderd aangezien deze is gespiegeld */
    RB5oud = PORTBbits.RB5;                                                     /*update RB5oud*/
    
    if(Controlle >> 7 == Kant)                                                  /*controleer of de juiste richting op wordt gedraaid*/
    {
        Controlle++;                                                            /*Verhoog de controlle*/
    }
    else
    {
        Controlle = Kant << 7;                                                  /*Hier wordt de controlle gereset*/
    }
    
    if((Controlle << 1) >= (CONTROLLES << 1))                                   /*Verhoogt alleen de richting bij genoeg controlles*/
    {
        if(Kant == Rechts)                                                      /*afhankelijk van de kant wordt de teller verhoogd of verlaagd*/
        {
            Teller++;
        }
        else
        {
            if(Teller == 0)                                                     /*Zet de teller op de max waarde wanneer hij overflowt*/
            {
                Teller = SCHAAL*AANTALKANALEN - 1;
            }
            else
            {
                Teller--;
            }
        }
        Controlle = 0;
    }
    
    Kanaal = (unsigned int)(((Teller) % (AANTALKANALEN*SCHAAL))/SCHAAL);        /*Zet de teller om naar een van de kanalen*/
    PORTA = 0b1111 - (1 << Kanaal);                                             /*Zet de juiste led aan*/
    SchermUpdate |= 2;                                                          //schrijf de teller naar het eeprom
    eeprom_write(INPUTAD, Teller);
}

void Kalibreren(void)
{
    MinSpanning = 1023;
    MaxSpanning = 0;
    int i;
    int ADres;                                                                  //om de 10bit adc waarde bij te houden
    if(eeprom_read(MINSPANNINGH) >> 2 == CONTROLLECODE)                         //als er een waarde staat opgeslagen in eeprom
    {
        MinSpanning = eeprom_read(MINSPANNINGH) & 0b11;                         //dan wordt deze opgehaald
        MinSpanning <<= 8;
        MinSpanning += eeprom_read(MINSPANNINGL);
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
    }
    
    if(eeprom_read(MAXSPANNINGH) >> 2 == CONTROLLECODE)
    {
        MaxSpanning = eeprom_read(MAXSPANNINGH) & 0b11;                         //lees de MinSpanning van EEPROM
        MaxSpanning <<= 8;
        MaxSpanning += eeprom_read(MAXSPANNINGL);
    }
    else
    {
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
}

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
        }
}

void spiWord(char Word) //Write data to SPI bus
{
    PORTDbits.RD7 = 0b1;    //zet register select hoog
    PORTCbits.RC7 = 0b1;    //zet register select hoog
    PORTDbits.RD4 = 0b0;    //zet ce laag
    PORTCbits.RC4 = 0b0;    //zet ce laag
    SSPBUF = Word;          //stuur bits
    while(!BF);             //wacht tot de transmissie klaar is

    PORTDbits.RD4 = 0b1;    //zet ce hoog voor latch
    PORTCbits.RC4 = 0b1;    //zet ce hoog voor latch
}

void spiWriteVol(int Getal)
{    
    PORTCbits.RC7 = 0b0;    //zet register select laag
    PORTCbits.RC4 = 0b0;    //zet register select laag
    
    //stuur aparte letters
    spiWrite(V);
    spiWrite(O);
    spiWrite(L);
    spiWrite(PUNT);
    spiWrite(SPATIE);
    spiWrite(SPATIE);
    
    spiWrite((Getal - Getal%10)/10);    //bereken welk getal er als eerste op het scherm moet staan en stuur het
    spiWrite(Getal%10);                 //bereken welk getal er als eerste op het scherm moet staan en stuur het
    PORTCbits.RC4 = 0b1;                //zet hoog voor latch
}

void spiWrite(int Getal)
{
    int i;
    for(i = 0; i < 5; i++)              //lees hele array waarde af om te sturen
    {
        SSPBUF = Letters[Getal][i];     //stuur bits
        while(!BF);                     //wacht to SPI klaar is
    }
}

void spiWriteInput(int Getal)
{
    PORTDbits.RD7 = 0b0;    //zet register select laag

    PORTDbits.RD4 = 0b0;    //zet latch laag
    
    //stuur aparte letters
    spiWrite(I);
    spiWrite(N);
    spiWrite(P);
    spiWrite(U);
    spiWrite(T);
    spiWrite(DUBBELEPUNT);
    spiWrite(SPATIE);
    spiWrite(Getal);        //schrijf het gewenste getal
    PORTDbits.RD4 = 0b1;    //zet hoog voor latch
}

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
                    
                unsigned i2;    
                    for(i2=0;i2<8;i2++)
                    {
                            __delay_us(RECEIVERCYCLE);          //wacht en zet de bits van de receiver in Opslag 1
                            Opslag1 <<= 1;
                            Opslag1 = PORTBbits.RB0;
                    }
                __delay_us(80 * RECEIVERCYCLE);                 //wacht weer tot het volgende continous signaal
                __delay_us(3 * RECEIVERCYCLE);                  //lees niet de sign bits (010) in want die zijn nutteloos nu
                
                for(i2=0;i2<8;i2++)
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

//Final Document