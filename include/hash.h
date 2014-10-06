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

#ifndef HASH_H
#define HASH_H

#include "hashInterface.h"
#include "sha256.h"

/**
 * Hash is an implementation of hashInterface
 */
class Hash : public hashInterface {
public:
	/**
	 * Hash() constructor
	 */
	Hash();

	/**
	 * hash() hashes "size" bytes of data at "data" and places the results
	 * in the location pointed to by "buffer"
	 */
	void hash(const void* data, uint8_t* buffer, uint8_t size);
};

#endif // HASH_H
