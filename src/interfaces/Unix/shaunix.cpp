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

#include "shaunix.h"
#include "string.h"
#include <stdio.h>

/**
 * Hash constructor
 */
Hash::Hash() {}

/**
 * hash() hashes "size" data pointed to by "data" and places the result in "buffer"
 */
void Hash::hash(const void* data, uint8_t* buffer, uint8_t size)
{
	SHA256 sha256;
	std::string myString (sha256((char*)data, size));
	char const *c = myString.c_str();
	uint8_t hashBuffer[32];
	hex2bin(c, (char*)hashBuffer);
	memcpy(buffer, hashBuffer, 32);
}

/**
 * char2int() converts a hex char "input" to it's int equivilent
 */
int Hash::char2int(char input)
{
  if(input >= '0' && input <= '9')
    return input - '0';
  if(input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if(input >= 'a' && input <= 'f')
    return input - 'a' + 10;
}

/**
 * hex2bin() converts a hex char "src" to its binary equivilent and puts the result in "target"
 */
void Hash::hex2bin(const char* src, char* target)
{
  while(*src && src[1])
  {
    *(target++) = char2int(*src)*16 + char2int(src[1]);
    src += 2;
  }
}