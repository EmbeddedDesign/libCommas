#include "avr/io.h"
#include "util/delay.h"
#include "eeprom_mem.h"
#include "serial_transport.h"
#include "hash.h"
#include "commas.h"

// Instantiate interface implementations
eeprom_mem onBoardEEPROM;
SerialTransport serial;
Hash sha;

// Instantiate Commas library
Commas saife(&onBoardEEPROM, &serial, &sha);

extern "C" void __cxa_pure_virtual() {
  for(;;);
}

// randomNumber() reads from floating pin A0 into ADC and returns a uint16_t value.
uint16_t randomNumber()
{	
	// Analog read of channel 0
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));   //16Mhz/128 = 125Khz the ADC reference clock
	ADMUX |= (1<<REFS0);                			//Voltage reference from Avcc (5v)
	ADCSRA |= (1<<ADEN);                			//Turn on ADC
	ADCSRA |= (1<<ADSC);    						//Defines the new ADC channel to be read
	ADMUX &= 0xF0;                    				//Clear the older channel that was read
	ADMUX |= 0;                						//Defines the new ADC channel to be read
	ADCSRA |= (1<<ADSC);                			//Starts a new conversion
	while(ADCSRA & (1<<ADSC));            			//Wait until the conversion is done
	return ADCW;    								//Returns the ADC value of the chosen channel
}

/**
 * Run the end device side join sequence state machine
 */
int main(void)
{
	// Seed the RNG with a random(ish) value
	saife.seedRandom(randomNumber());

	// Set bit 5 of port B (Arduino digital pin 13) as output
	// DDRB = _BV(PORTB5);
	// PORTB ^= _BV(PORTB5);

	// This isn't the best possible implemenation of a state machine but it gets the job done
	uint8_t state = 0;
	for(;;)
	{
		switch(state)
		{
			case 0:
				for(uint8_t i = 0; i < 2; i ++)
				{
					saife.sendHelloMessage();
					_delay_ms(500);
				}
				state = 1;
				break;
			case 1:
				if(saife.verifyJoinRequest())
				{
					state = 2;
				}
				break;
			case 2:
				for(uint8_t i = 0; i < 2; i ++)
				{
					saife.sendJoinResponse();
					_delay_ms(500);
				}
				state = 3;
				break;
			case 3:
				if(saife.verifyTokenSignature())
				{
					state = 4;
				}
				break;
			case 4:
				for(uint8_t i = 0; i < 2; i ++)
				{
					saife.sendACK();
					_delay_ms(500);
				}
				state = 5;
				break;
			case 5:
				static uint8_t data[] = { 'B', 'C', 'H'};
				saife.sendSignedMessage(data, sizeof(data));
				_delay_ms(500);
				break;
		}
	}
	return 0;
}