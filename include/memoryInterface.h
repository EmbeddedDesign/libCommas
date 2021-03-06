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

#ifndef MEMORYINTERFACE_H_
#define MEMORYINTERFACE_H_

#include "stdint.h"

/**
 * This pure virtual class represents a memory interface.
 */
class memoryInterface {
public:
	/**
   * Performs a read from memory.
   * write() "size" bytes of data from "pData" to location "id"
   */
	virtual bool write(uint8_t *pData, uint16_t size, uint8_t id) = 0;
	
	/**
   * Performs a write to memory.
   * read() "size" bytes of data from location "id" and place result at "pData"
   */
	virtual bool read(uint8_t *pData, uint16_t size, uint8_t id) = 0;
};

#endif // MEMORYINTERFACE_H_