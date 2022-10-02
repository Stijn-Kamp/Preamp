/* 
 * File:   Init.h
 * Author: timsp
 *
 * Created on 27 februari 2019, 16:28
 */

#ifndef INIT_H
#define	INIT_H

#ifdef	__cplusplus
extern "C" {
#endif

   
    
void Init(void);                                                                //Initialisatie van de PIC

void UpdateKanalen(void);                                                       /*Update het kanaal(LED's) na een verandering*/

void spiInit(void);                                                             //initialiseer SPI

void Kalibreren(void);                                                          //Kalibreren van de potmeter

void UpdateVolume(void);                                                        //Updaten van het volume

void spiWord(char Word);                                                        //functie om config word te sturen naar de displays

void spiWriteVol(int Getal);                                                    //fuctie die aangeroepen wordt om het volume naar het display te schrijven

void spiWrite(int Getal);                                                       //functie voor het schrijven naar SPI register

void spiWriteInput(int Getal);                                                  //fuctie die aangeroepen wordt om de input naar het display te schrijven

void UpdateAfstand(void);                                                       //functie die het schrijven naar de displays regelt


#ifdef	__cplusplus
}
#endif

#endif	/* INIT_H */

