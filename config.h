//  File config.h

//  Author: Renzo Dani
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.

#include <avr/io.h>
#include "Keypad.h"

#ifndef CONFIG_H
#define CONFIG_H
//*************************************************************
#define PB0 0
#define PB1 1
#define PB2 2
#define PB3 3
#define PB4 4
#define PB5 5

#define PC0 0
#define PC1 1
#define PC2 2
#define PC3 3

#define PD3 3
#define PD4 4
#define PD5 5
#define PD6 6
#define PD7 7

//////////////////////for SPI/////////////////////////////////////////

#define SPIDO	PB3	// Port B bit 3 : data out (data to MSD and vs1053)
#define SPIDI	PB4	// Port B bit 4 : data in (data from MSD and vs1053)
#define SPICLK	PB5	// Port B bit 5 : clock for MSD and vs1053


#define SPIPutCharWithoutWaiting(data) SPDR =data
#define SPIWait() while(!(SPSR & (1<<SPIF)))
void SPIPutChar(unsigned char data);
unsigned char SPIGetChar();
#define SPI8Clocks(nClocks)	{for(int i = 0; i < nClocks; i++){SPIPutChar(0xAA);}}

void InitSPI();


////////////////////////for MMC////////////////////////////////////

#define SPICS	PB2	// Port B bit 2 (pin14): chip select for MSD
#define DeselectMSD()	PORTB |= 1 << SPICS
#define SelectMSD()	PORTB &= ~(1 << SPICS)
#define SD_CS_PIN 10  //SD card select pin linked to pin10 of MCU



#define READ_SPIDI()	PINB & (1 << SPIDI)

///////////////////////for vs10xx///////////////////////////////////


#define VS_XRESET PC0
#define VS_DREQ PC1
#define VS_XDCS PC2
#define VS_XCS  PC3


#define VS_PORT PORTC
#define VS_PIN PINC
#define VS_DDR DDRC


#define Mp3SelectControl()	VS_PORT&= ~(1<<VS_XCS)
#define Mp3DeselectControl()	VS_PORT |= (1<<VS_XCS) 	


#define Mp3PutInRest() VS_PORT &= ~(1<<VS_XRESET) 
#define Mp3ReleaseFromReset() VS_PORT |= (1<<VS_XRESET)
#define Mp3SelectData() VS_PORT &= ~(1<<VS_XDCS)
#define Mp3DeselectData() VS_PORT |= (1<<VS_XDCS)

#define MP3_DREQ (VS_PIN & (1<<VS_DREQ))



void InitIOForVs10xx();


/////////////////////for keys//////////////////////////////////////

extern Keypad kpd;

//////////////////for LEDs///////////////////////////////////////

#define RED_LED 	PB1
#define GREEN_LED 	PB0
#define LED_PORT	PORTB
#define LED_DDR		DDRB

/*
#define RED_LED_ON()	LED_PORT |=(1<<RED_LED)
#define RED_LED_OFF()	LED_PORT &=~(1<<RED_LED)
#define GREEN_LED_ON()	LED_PORT |=(1<<GREEN_LED)
#define GREEN_LED_OFF()	LED_PORT &=~(1<<GREEN_LED)
*/
#define RED_LED_ON()	1
#define RED_LED_OFF()	1
#define GREEN_LED_ON()	1
#define GREEN_LED_OFF()	1


/** Generic temp variable for non-reentrant main routines */
union Temp {
  unsigned char c;
  unsigned int i;
  unsigned long l;
};
extern union Temp  temp;
#endif

