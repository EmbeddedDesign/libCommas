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

#include "file_mem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdint.h>

// We need to default to somewhere
// We can probably find a better location
#ifndef directory
    #define directory  "/usr/home/"
#endif

using namespace std;

/**
 * write() writes "size" bytes of data at "pData" to location "id"
 */
bool file_mem::write(uint8_t *pData, uint16_t size, uint8_t id)
{
    // set up the file name
    stringstream ss;
    ss << (int)id;
    std::string filename = ss.str();
    filename = directory + filename; //modify the path name here

    ofstream myfile;
    myfile.open (filename.c_str(), ios::out | ios::binary);

    myfile.write((char*)pData,size);
    myfile.close();

    return true;
}

/**
 * read() reads "size" bytes of data at location "id" and puts the result at "pData"
 */
bool file_mem::read(uint8_t *pData, uint16_t size, uint8_t id)
{
    stringstream ss;
    ss << (int)id;
    std::string filename = ss.str();
    filename = directory + filename;   //modify the path name here
    ifstream myfile;

    myfile.open (filename.c_str(), ios::in | ios::binary);

    myfile.read((char*)pData,size);

    myfile.close();

	return true;
}