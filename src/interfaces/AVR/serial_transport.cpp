/*
Copyright 2014 SAIFE, Inc.
https://saifeinc.com/

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**
 * Authors: Breck Boven, John Curtis, Stephen Harper
**/

#include "serial_transport.h"
#include "avr/io.h"
#include "tinyserial.h"

#ifndef BAUD_RATE
 #define BAUD_RATE 19200
#endif

#if(BAUD_RATE == 1200)
	#define BAUD TS_CONFIG_16MHZ_1200BAUD
#endif
#if(BAUD_RATE == 2400)
	#define BAUD TS_CONFIG_16MHZ_2400BAUD
#endif
#if(BAUD_RATE == 9600)
	#define BAUD TS_CONFIG_16MHZ_9600BAUD
#endif
#if(BAUD_RATE == 14400)
	#define BAUD TS_CONFIG_16MHZ_14400BAUD
#endif
#if(BAUD_RATE == 19200)
	#define BAUD TS_CONFIG_16MHZ_19200BAUD
#endif
#if(BAUD_RATE == 28800)
	#define BAUD TS_CONFIG_16MHZ_28800BAUD
#endif
#if(BAUD_RATE == 38400)
	#define BAUD TS_CONFIG_16MHZ_38400BAUD
#endif

/**
 * SerialTransport constructor
 */
SerialTransport::SerialTransport()
{
	ts_init(BAUD, TS_MODE_READWRITE);
}

/**
 * Read() attempts to read "max_read" bytes from the serial port and store the result at "buffer"
 * Currently a blocking function. This could probably be improved.
 */
uint8_t *SerialTransport::Read(uint8_t* buffer, const uint8_t max_read)
{
	for(uint8_t i = 0; i < max_read; i++)
	{
		buffer[i] = ts_getc(TS_ECHO_OFF);
	}
	return buffer;
}

/**
 * Write() writes "length" bytes of data from the location pointed to by "buffer" to the serial port
 */
void SerialTransport::Write(const uint8_t* buffer, const uint8_t length)
{
	for(uint8_t i = 0; i < length; i++)
	{
		ts_putc(buffer[i]);
	}
}

/**
 * Close() closes the serial connection
 * Currently does nothing as it is unnecessary but requires an implementation due to the interface
 */
void SerialTransport::Close()
{

}