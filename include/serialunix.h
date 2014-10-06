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

#ifndef SERIALUNIX_H
#define SERIALUNIX_H

#include <stdint.h>
#include "transportInterface.h"

/**
 * serialunix is an implementation of transportInterface
 * for Unix like operating systems
 */
class serialunix : public transportInterface {
public:
	/**
	 * serialunix() constructor
	 */
	serialunix();

	/**
	 * Read() attempts to read "max_read" bytes from serial and place the result at "buffer"
	 */
	uint8_t* Read(uint8_t* buffer, const uint8_t max_read);

	/**
	 * Write() attempts to write "length" bytes at "buffer" to serial
	 */
	void Write(const uint8_t* buffer, const uint8_t length);
	
	/**
	 * Close() closes a serial connection
	 */
	void Close();
	
private:
	/**
	 * set_interface_attribs() sets serial interface attributes
	 */
	int set_interface_attribs (int fd, int speed, int parity);

	/**
	 * open_serial() opens a serial connection
	 */
	void open_serial(void);
};

#endif