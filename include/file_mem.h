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

#ifndef FILE_MEM_H
#define FILE_MEM_H

#include <stdint.h>
#include "memoryInterface.h"

/**
 * file_mem is a memoryInterface implementation for platforms on which data
 * can be stored in the form of files.
 */
class file_mem : public memoryInterface {
public:
	/**
	 * write() writes "size" data pointed to by "pData" to location "id"
	 */
	bool write(uint8_t *pData, uint16_t size, uint8_t id);
	
	/**
	 * read() reads "size" data from location "id" into buffer pointed to by "pData"
	 */
	bool read(uint8_t *pData, uint16_t size, uint8_t id);
};

#endif