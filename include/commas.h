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

#ifndef COMMAS_H
#define COMMAS_H

#include "transportInterface.h"
#include "memoryInterface.h"
#include "hashInterface.h"
#include "uECC.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "authExchProto.pb.h"
#include "stdint.h"
#include "stdlib.h"

/**
 * The size of a private key. Public keys and signatures should be twice as long
 */
#define key_size 32

/**
 * class Commas contains all of the functions necessary to complete an authenticated join sequence and send signed/unsigned messages
 */
class Commas {
public:
    /**
     * A pointer to the memory interface
     */
	memoryInterface *pMEMORYInterface;

    /**
     * A pointer to the transport interface
     */
    transportInterface *pTRANSPORTInterface;

    /**
     * A pointer to the hash interface
     */
    hashInterface *pHASHInterface;

    /**
     * Constructor needs access to the memory, transport, and hash interface implementations
     */
	Commas(memoryInterface *MEMORYinterface, transportInterface *TRANSPORTInterface, hashInterface *HASHInterface);

    /**
     * Destructor
     */
    ~Commas();

    /**
     * Seeds the random number generator
     */
    void seedRandom(uint16_t randomNumber);

    /**
     * Broadcast a hello message
     */
    void sendHelloMessage();

    /**
     * Verify a hello message
     */
    bool verifyHelloMessage();

    /**
     * Send a join request
     */
    void sendJoinRequest();

    /**
     * Check to see if a message is received and determine if it is a join request
     */
    bool verifyJoinRequest();

    /**
     * Send a join response
     */
    void sendJoinResponse();

    /**
     * Verify a join response
     */
    bool verifyJoinResponse();

    /**
     * Send signature of token
     */
    void sendTokenSignature();

    /**
     * Verify signature of token
     */
    bool verifyTokenSignature();

    /**
     * Send ACK
     */
    void sendACK();

    /**
     * Verify ACK
     */
    bool verifyACK();

    /**
     * Send a signed message
     */
    void sendSignedMessage(uint8_t* data, uint8_t size);

    /**
     * Verify a signed message
     */
    bool verifySignedMessage(uint8_t* data);

    /**
     * Send an unsigned message
     */
    void sendUnsignedMessage(uint8_t* data, uint8_t size);

    /**
     * Sign Data
     */
    void sign(const uint8_t* data, uint8_t* buffer, uint8_t size);

private:
    /**
     * Verify a Signature of Data with the Public Key Provided
     */
    bool verify(const uint8_t* pubK, const uint8_t* data, const uint8_t* sig, uint8_t size);

    /**
     * Hash data and put the result in buffer
     */
    void hash(const uint8_t* data, uint8_t* buffer, uint8_t size);

    /**
     * Read "size" bytes from memory location "id" and place the result in "*pData"
     */
    void readMem(uint8_t *pData, uint16_t size, uint8_t id);

    /**
     * Holds a random token generated at instantiation
     */
    uint8_t token[4];

    /**
     * Holds a random token received from another device
     */
    uint8_t tokenBuffer[4];
};

/**
 * class Commas_FACTORY contains functions necessary to write and check initial key values
 */
class Commas_FACTORY {
public:
    /**
     * A pointer to the memory interface
     */
    memoryInterface *pMEMORYInterface;
    
    /**
     * Constructor only needs access to the memory interface implementation
     */
    Commas_FACTORY(memoryInterface *MEMORYinterface);

    /**
     * Set Certificate Authority Public Key
     */
    void setCA(const char* CA);

    /**
     * Set Public Key
     */
    void setPublicKey(const char* pubK);

    /**
     * Set Private Key
     */
    void setPrivateKey(const char* prvK);

    /**
     * Set UUID
     */
    void setUUID(const char* uuid);

    /**
     * Set Device Description
     */
    void setDescription(const char* description);

    /**
     * Set Attestation Signature
     */
    void setAttestationSig(const char* attSig);

    /**
     * Return Certificate Authority Public Key
     */
    uint8_t *getCA();

    /**
     * Return Local Public Key
     */
    uint8_t *getPublicKey();

    /**
     * Return UUID
     */
    uint8_t *getUUID();

    /**
     * Return Description
     */
    uint8_t *getDescription();

    /**
     * Return Attestation Signature
     */
    uint8_t *getAttestationSig();

    /**
     * Read "size" bytes from memory location "id" and place the result in "*pData"
     */
    void readMem(uint8_t *pData, uint16_t size, uint8_t id);

private:
    /**
     * Serves to reduce repeated code called by setCA, setPublicKey, setPrivateKey, setAttestationSig
     */
    void setKey(const char* key, uint8_t keySize, uint8_t id);

    /**
     * A buffer to store uint8_t arrays of keys read from memory
     */
    uint8_t buffer[64];
};

#endif // COMMAS_H