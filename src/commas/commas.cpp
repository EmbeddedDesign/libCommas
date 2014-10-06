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

#include "commas.h"

extern "C" {

/**
 * RNG() defines the random number generator function for uECC
 */
static int RNG(uint8_t *p_dest, unsigned p_size)
{
  while(p_size) {
    long v = random();
    unsigned l_amount;
    if(p_size < sizeof(long)) {
      l_amount = p_size;
    }
    else {
      l_amount = sizeof(long);
    }
    memcpy(p_dest, &v, l_amount);
    p_size -= l_amount;
    p_dest += l_amount;
  }
  return 1;
}

}

/**
 * Commas() constructor
 * takes in pointers to instances of implementations of memoryInterface, transportInterface, hashInterface
 */
Commas::Commas(memoryInterface *MEMORYInterface, transportInterface *TRANSPORTInterface, hashInterface *HASHInterface)
{
	pMEMORYInterface = MEMORYInterface;
	pTRANSPORTInterface = TRANSPORTInterface;
  pHASHInterface = HASHInterface;

  for(uint8_t i = 0; i < sizeof(token); i++)
  {
    token[i] = (uint8_t)random();
  }
}

/**
 * Commas() destructor
 */
Commas::~Commas()
{

}

/**
 * seedRandom() seeds random with "randomNumber" and sets the RNG for uECC
 */
void Commas::seedRandom(uint16_t randomNumber)
{
  srandom(randomNumber);
  uECC_set_rng(&RNG);
}

/**
 * verify a signature
 * takes in a public key, hash, and signature and verifies that the
 */
bool Commas::verify(const uint8_t* pubK, const uint8_t* data, const uint8_t* sig, uint8_t size)
{
  uint8_t hashBuffer[32];
  hash(data, hashBuffer, size);

	return uECC_verify(pubK, hashBuffer, sig);
}
    
/**
 * sign() "size" bytes of data at "data" and place the result at "buffer"
 */
void Commas::sign(const uint8_t* data, uint8_t* buffer, uint8_t size)
{
  uint8_t hashBuffer[32];
  hash(data, hashBuffer, size);

  uint8_t prvKBuffer[32];
  readMem(prvKBuffer, sizeof(prvKBuffer), 3);
 
  uECC_sign(prvKBuffer, hashBuffer, buffer);
}
  
/**
 * SHA256 hash of data
 * hash() "size" bytes of data at "data" and place the result at "buffer"
 */
void Commas::hash(const uint8_t* data, uint8_t* buffer, uint8_t size)
{
  pHASHInterface->hash(data, buffer, size);
}

/**
 * sendHelloMessage() sends a hello message
 */
void Commas::sendHelloMessage()
{
  authExchProto msg;

	memcpy(msg.msgType.bytes, "1", 1);
  msg.msgType.size = 1;

  uint8_t CAbuf[64];
  readMem(CAbuf, sizeof(CAbuf), 1);

  memcpy(msg.data1.bytes, CAbuf, sizeof(CAbuf));
  msg.has_data1 = true;
  msg.data1.size = sizeof(CAbuf);
    
  uint8_t UUIDbuf[16];
  readMem(UUIDbuf, sizeof(UUIDbuf), 4);

  memcpy(msg.data2.bytes, UUIDbuf, sizeof(UUIDbuf));
  msg.has_data2 = true;
  msg.data2.size = sizeof(UUIDbuf);

  msg.has_data3 = false;
  msg.has_data4 = false;

  pb_ostream_t sizestream = {0};
  pb_encode(&sizestream, authExchProto_fields, &msg);

  uint8_t message[sizestream.bytes_written];

  pb_ostream_t stream = pb_ostream_from_buffer(message, sizestream.bytes_written);
  pb_encode(&stream, authExchProto_fields, &msg);

  pTRANSPORTInterface->Write(message, sizestream.bytes_written);
}

/**
 * verifyHelloMessage() verifies a hello message
 */
bool Commas::verifyHelloMessage()
{
  authExchProto msg;

  uint8_t buffer[64];
  readMem(buffer, 64, 1);

  uint8_t message[authExchProto_size];
  pTRANSPORTInterface->Read(message, sizeof(message));

  pb_istream_t stream = pb_istream_from_buffer(message, sizeof(message));
  bool isValidMessage = pb_decode(&stream, authExchProto_fields, &msg);

  if(isValidMessage && msg.has_data1 && msg.has_data2 && msg.msgType.bytes[0] == (uint8_t)'1')
  {
    for(uint8_t i = 0; i < 64; i++)
    {
      if(msg.data1.bytes[i] != buffer[i])
      {
        return false;
      }
    }
    // May want to add check of UUID prefix here
    return true;
  }
  return false;
}

/**
 * sendJoinRequest() sends a join request message
 */
void Commas::sendJoinRequest()
{
  authExchProto msg;

  memcpy(msg.msgType.bytes, "2", 1);
  msg.msgType.size = 1;

  uint8_t CAbuf[64];
  readMem(CAbuf, sizeof(CAbuf), 1);

  memcpy(msg.data1.bytes, CAbuf, sizeof(CAbuf));
  msg.has_data1 = true;
  msg.data1.size = sizeof(CAbuf);
    
  uint8_t PUBKbuff[64];
  readMem(PUBKbuff, sizeof(PUBKbuff), 2);

  memcpy(msg.data2.bytes, PUBKbuff, sizeof(PUBKbuff));
  msg.has_data2 = true;
  msg.data2.size = sizeof(PUBKbuff);

  uint8_t ATTSIGbuff[64];
  readMem(ATTSIGbuff, sizeof(ATTSIGbuff), 6);

  memcpy(msg.data3.bytes, ATTSIGbuff, sizeof(ATTSIGbuff));
  msg.has_data3 = true;
  msg.data3.size = sizeof(ATTSIGbuff);

  memcpy(msg.data4.bytes, &token, sizeof(token));
  msg.has_data4 = true;
  msg.data4.size = sizeof(token);

  pb_ostream_t sizestream = {0};
  pb_encode(&sizestream, authExchProto_fields, &msg);

  uint8_t message[sizestream.bytes_written];

  pb_ostream_t stream = pb_ostream_from_buffer(message, sizestream.bytes_written);
  pb_encode(&stream, authExchProto_fields, &msg);

  pTRANSPORTInterface->Write(message, sizestream.bytes_written);
}

/**
 * verifyJoinRequest() verifies a join request
 */
bool Commas::verifyJoinRequest()
{
  authExchProto msg;

  uint8_t message[authExchProto_size+3];
  pTRANSPORTInterface->Read(message, sizeof(message));

  pb_istream_t stream = pb_istream_from_buffer(message, sizeof(message));
  bool isValidMessage = pb_decode(&stream, authExchProto_fields, &msg);

  if(isValidMessage && msg.has_data1 && msg.has_data2 && msg.has_data3 && msg.has_data4 && msg.msgType.bytes[0] == (uint8_t)'2')
  {
    uint8_t CAbuf[64];
    readMem(CAbuf, sizeof(CAbuf), 1);
    for(uint8_t i = 0; i < 64; i++)
    {
      if(msg.data1.bytes[i] != CAbuf[i])
      {
        return false;
      }
    }
    if(verify(CAbuf, msg.data2.bytes, msg.data3.bytes, msg.data2.size))
    {
      memcpy(&tokenBuffer, msg.data4.bytes, sizeof(tokenBuffer));
      pMEMORYInterface->write(msg.data2.bytes, msg.data2.size, 7);
      return true;
    }
    return false;
  }
  return false;
}

/**
 * sendJoinResponse() sends a join response message
 */
void Commas::sendJoinResponse()
{
  uint8_t SIGbuff[64];
  sign((uint8_t*)&tokenBuffer, SIGbuff, sizeof(token));

  authExchProto msg;

  memcpy(msg.msgType.bytes, "3", 1);
  msg.msgType.size = 1;

  uint8_t PubKbuff[64];
  readMem(PubKbuff, sizeof(PubKbuff), 2);

  memcpy(msg.data1.bytes, PubKbuff, sizeof(PubKbuff));
  msg.has_data1 = true;
  msg.data1.size = sizeof(PubKbuff);
    
  uint8_t ATTSIGbuff[64];
  readMem(ATTSIGbuff, sizeof(ATTSIGbuff), 6);

  memcpy(msg.data2.bytes, ATTSIGbuff, sizeof(ATTSIGbuff));
  msg.has_data2 = true;
  msg.data2.size = sizeof(ATTSIGbuff);

  memcpy(msg.data3.bytes, SIGbuff, sizeof(SIGbuff));
  msg.has_data3 = true;
  msg.data3.size = sizeof(SIGbuff);

  memcpy(msg.data4.bytes, &token, sizeof(token));
  msg.has_data4 = true;
  msg.data4.size = sizeof(token);

  pb_ostream_t sizestream = {0};
  pb_encode(&sizestream, authExchProto_fields, &msg);

  uint8_t message[sizestream.bytes_written];

  pb_ostream_t stream = pb_ostream_from_buffer(message, sizestream.bytes_written);
  pb_encode(&stream, authExchProto_fields, &msg);

  pTRANSPORTInterface->Write(message, sizestream.bytes_written);
}

/**
 * verifyJoinResponse() verifies a join response
 */
bool Commas::verifyJoinResponse()
{
  authExchProto msg;

  uint8_t buffer[64];
  readMem(buffer, sizeof(buffer), 1);

  uint8_t message[authExchProto_size+3];
  pTRANSPORTInterface->Read(message, sizeof(message));

  pb_istream_t stream = pb_istream_from_buffer(message, sizeof(message));
  bool isValidMessage = pb_decode(&stream, authExchProto_fields, &msg);

  if(isValidMessage && msg.has_data1 && msg.has_data2 && msg.has_data3 && msg.has_data4 && msg.msgType.bytes[0] == (uint8_t)'3')
  {
    if(verify(buffer, msg.data1.bytes, msg.data2.bytes, msg.data1.size))
    {
      if(verify(msg.data1.bytes, (uint8_t*)&token, msg.data3.bytes, 4))
      {
        pMEMORYInterface->write(msg.data1.bytes, 64, 7);
        memcpy(&tokenBuffer, msg.data4.bytes, 4);
        return true;
      }
      return false;
    }
    return false;
  }
  return false;
}

/**
 * sendTokenSignature() sends a token signature message
 */
void Commas::sendTokenSignature()
{
  authExchProto msg;

  memcpy(msg.msgType.bytes, "4", 1);
  msg.msgType.size = 1;

  uint8_t SIGbuff[64];
  sign((uint8_t*)&tokenBuffer, SIGbuff, 4);

  memcpy(msg.data1.bytes, SIGbuff, sizeof(SIGbuff));
  msg.has_data1 = true;
  msg.data1.size = sizeof(SIGbuff);

  msg.has_data2 = false;
  msg.has_data3 = false;
  msg.has_data4 = false;

  pb_ostream_t sizestream = {0};
  pb_encode(&sizestream, authExchProto_fields, &msg);

  uint8_t message[sizestream.bytes_written];

  pb_ostream_t stream = pb_ostream_from_buffer(message, sizestream.bytes_written);
  pb_encode(&stream, authExchProto_fields, &msg);

  pTRANSPORTInterface->Write(message, sizestream.bytes_written);
}

/**
 * verifyTokenSignature() verifies a token signature
 */
bool Commas::verifyTokenSignature()
{
  authExchProto msg;

  uint8_t message[authExchProto_size+3];
  pTRANSPORTInterface->Read(message, sizeof(message));

  pb_istream_t stream = pb_istream_from_buffer(message, sizeof(message));
  bool isValidMessage = pb_decode(&stream, authExchProto_fields, &msg);

  if(isValidMessage && msg.has_data1 && msg.msgType.bytes[0] == (uint8_t)'4')
  {
    uint8_t PUBKbuff[64];
    readMem(PUBKbuff, sizeof(PUBKbuff), 7);
    if(verify(PUBKbuff, (uint8_t*)&token, msg.data1.bytes, sizeof(token)))
    {
      return true;
    }
    return false;
  }
  return false;
}

/**
 * sendACK() sends an ACK message
 */
void Commas::sendACK()
{
  char ack[3] = { 'a', 'c', 'k' };

  authExchProto msg;

  memcpy(msg.msgType.bytes, "5", 1);
  msg.msgType.size = 1;

  uint8_t ACKbuff[64];
  sign((uint8_t*)ack, ACKbuff, sizeof(ack));

  memcpy(msg.data1.bytes, ACKbuff, sizeof(ACKbuff));
  msg.has_data1 = true;
  msg.data1.size = sizeof(ACKbuff);

  msg.has_data2 = false;
  msg.has_data3 = false;
  msg.has_data4 = false;

  pb_ostream_t sizestream = {0};
  pb_encode(&sizestream, authExchProto_fields, &msg);

  uint8_t message[sizestream.bytes_written];

  pb_ostream_t stream = pb_ostream_from_buffer(message, sizestream.bytes_written);
  pb_encode(&stream, authExchProto_fields, &msg);

  pTRANSPORTInterface->Write(message, sizestream.bytes_written);
}

/**
 * verifyACK() verifies an ACK
 */
bool Commas::verifyACK()
{
  char ack[3] = { 'a', 'c', 'k' };

  authExchProto msg;

  uint8_t message[authExchProto_size];
  pTRANSPORTInterface->Read(message, sizeof(message));

  pb_istream_t stream = pb_istream_from_buffer(message, sizeof(message));
  bool isValidMessage = pb_decode(&stream, authExchProto_fields, &msg);

  if(isValidMessage && msg.has_data1 && msg.msgType.bytes[0] == (uint8_t)'5')
  {
    uint8_t PUBKbuff[64];
    readMem(PUBKbuff, sizeof(PUBKbuff), 7);
    if(verify(PUBKbuff, (uint8_t*)ack, msg.data1.bytes, sizeof(ack)))
    {
      return true;
    }
    return false;
  }
  return false;
}

/**
 * sendSignedMessage() sends a signed message
 */
void Commas::sendSignedMessage(uint8_t* data, uint8_t size)
{
  uint8_t SigBuff[64];
  sign(data, SigBuff, size);

  authExchProto msg;

  memcpy(msg.msgType.bytes, "6", 1);
  msg.msgType.size = 1;

  memcpy(msg.data1.bytes, data, size);
  msg.has_data1 = true;
  msg.data1.size = size;

  memcpy(msg.data2.bytes, SigBuff, sizeof(SigBuff));
  msg.has_data2 = true;
  msg.data2.size = sizeof(SigBuff);

  msg.has_data3 = false;
  msg.has_data4 = false;

  pb_ostream_t sizestream = {0};
  pb_encode(&sizestream, authExchProto_fields, &msg);

  uint8_t message[sizestream.bytes_written];

  pb_ostream_t stream = pb_ostream_from_buffer(message, sizestream.bytes_written);
  pb_encode(&stream, authExchProto_fields, &msg);

  pTRANSPORTInterface->Write(message, sizestream.bytes_written);
}

/**
 * verifySignedMessage() verifies a signed message
 */
bool Commas::verifySignedMessage(uint8_t* data)
{
  authExchProto msg;

  uint8_t message[authExchProto_size+3];
  pTRANSPORTInterface->Read(message, sizeof(message));

  pb_istream_t stream = pb_istream_from_buffer(message, sizeof(message));
  bool isValidMessage = pb_decode(&stream, authExchProto_fields, &msg);

  if(isValidMessage && msg.has_data1 && msg.has_data2 && msg.msgType.bytes[0] == (uint8_t)'6')
  {
    uint8_t PUBKbuff[64];
    readMem(PUBKbuff, sizeof(PUBKbuff), 7);
    if(verify(PUBKbuff, msg.data1.bytes, msg.data2.bytes, msg.data1.size))
    {
      memcpy(data, msg.data1.bytes, msg.data1.size);
      return true;
    }
    return false;
  }
  return false;
}

/**
 * sendUnsignedMessage() sends and unsigned message
 */
void Commas::sendUnsignedMessage(uint8_t* data, uint8_t size)
{
  authExchProto msg;

  memcpy(msg.msgType.bytes, "7", 1);
  msg.msgType.size = 1;

  memcpy(msg.data1.bytes, data, size);
  msg.has_data1 = true;
  msg.data1.size = size;

  msg.has_data2 = false;
  msg.has_data3 = false;
  msg.has_data4 = false;

  pb_ostream_t sizestream = {0};
  pb_encode(&sizestream, authExchProto_fields, &msg);

  uint8_t message[sizestream.bytes_written];

  pb_ostream_t stream = pb_ostream_from_buffer(message, sizestream.bytes_written);
  pb_encode(&stream, authExchProto_fields, &msg);

  pTRANSPORTInterface->Write(message, sizestream.bytes_written);
}

/**
 * readMem() reads "size" bytes of data from location "id" and stores the result at "pData"
 */
void Commas::readMem(uint8_t *pData, uint16_t size, uint8_t id)
{
  pMEMORYInterface->read(pData, size, id);
}

/**
 * Commas_FACTORY() constructor
 * takes in a poninter to an instance of an implementation of memoryInterface
 */
Commas_FACTORY::Commas_FACTORY(memoryInterface *MEMORYInterface)
{
	pMEMORYInterface = MEMORYInterface;
}

/**
 * setCA() sets a CA public key
 */
void Commas_FACTORY::setCA(const char* CA)
{
  setKey(CA, key_size*2, 1);
}

/**
 * setPublicKey() sets a public key
 */
void Commas_FACTORY::setPublicKey(const char* pubK)
{
  setKey(pubK, key_size*2, 2);
}

/**
 * setPrivateKey() sets a private key
 */
void Commas_FACTORY::setPrivateKey(const char* prvK)
{
  setKey(prvK, key_size, 3);
}

/**
 * setUUID() sets a UUID
 */
void Commas_FACTORY::setUUID(const char* uuid)
{
  uint8_t buffer[16];
  char tmp[2];
  uint8_t j = 0;
  uint8_t i = 0;

  while(i < 36)
  {
  	if(uuid[i]=='-')
  	{
	    i++;
  	}
  	else
  	{
  		  tmp[0] = uuid[i];
  	    tmp[1] = uuid[i+1];
  	    buffer[j] = (uint8_t)strtoul(tmp,NULL,16);
  	    j++;
  	    i=i+2;
  	}
  }
  pMEMORYInterface->write(buffer, 16, 4);
}

/**
 *  setDescription() sets a device description
 */
void Commas_FACTORY::setDescription(const char* description)
{
  char buffer[64];

  for(uint8_t i = 0; i < 64; i++)
  {
    buffer[i] = description[i];
  }
  pMEMORYInterface->write((uint8_t*)buffer, 64, 5);
}

/**
 * setAttestationSig() sets an attestation signature
 */
void Commas_FACTORY::setAttestationSig(const char* attSig)
{
  setKey(attSig, key_size*2, 6);
}
 
/**
 * setKey() sets a key
 */
void Commas_FACTORY::setKey(const char* key, uint8_t keySize, uint8_t id)
{
  uint8_t buffer[keySize];
  char tmp[2];
  memset (buffer, 0, sizeof(buffer));
  uint8_t j=0;

  for(uint8_t i = 0; i < keySize*2; i+=2)
  {
    tmp[0] = key[i];
    tmp[1] = key[i+1];
    buffer[j] = (uint8_t)strtoul(tmp,NULL,16);
    j++;
  }
  pMEMORYInterface->write(buffer, sizeof(buffer), id);
}
  
/**
 * getCA() returns the certificate authority public key
 */
uint8_t *Commas_FACTORY::getCA()
{
  readMem(buffer, 64, 1);
  return buffer;
}
 
/**
 * getPublicKey() returns the certificate authority public key
 */
uint8_t *Commas_FACTORY::getPublicKey()
{
  readMem(buffer, 64, 2);
  return buffer;
}
 
/**
 * getUUID() returns the UUID
 */
uint8_t *Commas_FACTORY::getUUID()
{
  readMem(buffer, 16, 4);
  return buffer;
}
 
/**
 * getDescription() returns the device description
 */
uint8_t *Commas_FACTORY::getDescription()
{
  readMem(buffer, 64, 5);
  return buffer;
}

/**
 * getAttestationSig() returns the attestation signature
 */
uint8_t *Commas_FACTORY::getAttestationSig()
{
  readMem(buffer, 64, 6);
  return buffer;
}

/**
 * readMem() reads "size" bytes of data at location "id" and places the result at "pData"
 */
void Commas_FACTORY::readMem(uint8_t *pData, uint16_t size, uint8_t id)
{
  pMEMORYInterface->read(pData, size, id);
}