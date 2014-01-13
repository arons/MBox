//  File config.cpp

#include <avr/io.h>
#include "config.h"
#include "Keypad.h"

union Temp temp;


const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};
byte rowPins[ROWS] = {7, 2, 3, 6}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {5, 8, 4}; //connect to the column pinouts of the kpd

//byte rowPins[ROWS] = {9, 2, 3, 8}; //connect to the row pinouts of the kpd
//byte colPins[COLS] = {5, 10, 4}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void InitSPI()
{
	DDRB &= ~(1 << SPIDI);
	DDRB |= (1 << SPIDO);
	DDRB |= (1 << SPICLK);
	#if defined(__AVR_ATmega1280__)||defined(__AVR_ATmega2560__)
	DDRB |= (1 << SPICS) |(1<<PB0) ;//PB0 is must, because it the SS of SPI of the ATMega1280
	#else
	DDRB |= (1 << SPICS);
	#endif

	SPCR = (1 << SPE) | (1 << MSTR) |(1 << SPR0);// | (1 << SPR0) ;//SPICLK=CPU/16
    SPSR = 0;
	
	//PORTB |= (1 << SPICS)|(1<<PB0);//deselect mmc when initial
    PORTB |= (1 << SPICS);
    //Serial.println("Initial SPI ok!");
}

void SPIPutChar(unsigned char data)
{
	SPDR =data;
	while(!(SPSR & (1<<SPIF)));
}
unsigned char SPIGetChar()
{
  unsigned char data = 0;
  SPDR =0xFF;
  SPIWait();
  data = SPDR;
  return data;
}

//Initialize IO control ports of vs10xx
void InitIOForVs10xx()
{
  VS_DDR &= ~(1 << VS_DREQ);//input
  VS_DDR |= (1 << VS_XDCS);//output
  VS_DDR |= (1 << VS_XCS);//output
  VS_DDR |= (1 << VS_XRESET);//output
  
  VS_PORT |= 	(1 << VS_XDCS);//deselect vs_xdcs 
  VS_PORT |= 	(1 << VS_XCS);//deselect vs_xcs

}

