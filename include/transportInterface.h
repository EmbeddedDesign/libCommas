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
 * Modified from code originally written by Anthony Payne
**/

#ifndef TRANSPORT_INTERFACE_H_
#define TRANSPORT_INTERFACE_H_

#include <stdint.h>

/**
 * This pure virtual class represents a transport (or connection).
 */
class transportInterface {
public:
  /**
   * Performs a blocking read on this transport.
   *
   * @param buffer_ptr To hold the data read from the transport
   * @param offset Location in the buffer to put the data read from the transport
   * @param max_read Maximum number of bytes to read from the underlying transport
   * @return Number of bytes read
   */
  virtual uint8_t* Read(uint8_t* buffer, const uint8_t max_read) = 0;

  /**
   * Performs a blocking write on this transport
   *
   * @param buffer Buffer holding data to write to the transport
   * @param offset Offset into buffer to start from
   * @param length Number of bytes from buffer to write to the transport
   */
  virtual void Write(const uint8_t* buffer, const uint8_t length) = 0;

  /**
   * Closes the transport
   */
  virtual void Close() = 0;

  /**
   * @return True if this transport is closed and is no longer usable. False otherwise.
   */
  bool closed() const {
    return closed_;
  }

 protected:
  /** Flag indicating if the transport is closed and no longer usable or not */
  bool closed_;
};

#endif  // TRANSPORT_INTERFACE_H_