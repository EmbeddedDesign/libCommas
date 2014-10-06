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

#ifndef EEPROM_MEM_H
#define EEPROM_MEM_H

#include "stdint.h"
#include "memoryInterface.h"

/**
 * eeprom_mem is an implementatino of memoryInterface
 * that uses EEPROM memory as a non-volatile storage medium
 */
class eeprom_mem : public memoryInterface {
public:
	/**
	 * eeprom_mem() constructor
	 */
	eeprom_mem();
	
	/**
	 * write() "size" bytes of data from "pData" to location "id"
	 */
	bool write(uint8_t *pData, uint16_t size, uint8_t id);
	
	/**
	 * read() "size" bytes of data from location "id" and place the result at "pData"
	 */
	bool read(uint8_t *pData, uint16_t size, uint8_t id);
};

#endif