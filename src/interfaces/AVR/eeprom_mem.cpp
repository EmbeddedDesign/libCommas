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

#include "eeprom_mem.h"
#include "avr/eeprom.h"

/**
 * eeprom_mem constructor
 */
eeprom_mem::eeprom_mem() {}

/**
 * write() writes "size" bytes of data pointed to by "pData" to location "id"
 * eeprom memory addresses are hardcoded to acomodate the amount of data they must store
 */
bool eeprom_mem::write(uint8_t *pData, uint16_t size, uint8_t id) {
	switch (id)
	{
		case 1:
			eeprom_write_block((void*)pData, (void*)1, size);
			break;
		case 2:
			eeprom_write_block((void*)pData, (void*)65, size);
			break;
		case 3:
			eeprom_write_block((void*)pData, (void*)129, size);
			break;
		case 4:
			eeprom_write_block((void*)pData, (void*)161, size);
			break;
		case 5:
			eeprom_write_block((void*)pData, (void*)177, size);
			break;
		case 6:
			eeprom_write_block((void*)pData, (void*)241, size);
			break;
		case 7:
			eeprom_write_block((void*)pData, (void*)305, size);
			break;
	}
	return true;
}

/**
 * read() reads "size" bytes of data from location "id" and puts the result at "pData"
 * eeprom memory addresses are hardcoded to acomodate the amount of data they must store
 */
bool eeprom_mem::read(uint8_t *pData, uint16_t size, uint8_t id) {
	switch (id)
	{
		case 1:
			eeprom_read_block((void*)pData, (void*)1, size);
			break;
		case 2:
			eeprom_read_block((void*)pData, (void*)65, size);
			break;
		case 3:
			eeprom_read_block((void*)pData, (void*)129, size);
			break;
		case 4:
			eeprom_read_block((void*)pData, (void*)161, size);
			break;
		case 5:
			eeprom_read_block((void*)pData, (void*)177, size);
			break;
		case 6:
			eeprom_read_block((void*)pData, (void*)241, size);
			break;
		case 7:
			eeprom_read_block((void*)pData, (void*)305, size);
			break;
	}
	return true;
}