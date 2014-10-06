#include "commas.h"
#include "file_mem.h"
#include "serialunix.h"
#include "shaunix.h"
#include "temp_key_file.h"
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

/**
 * Instantiate interface implementations
 */
file_mem memfile;
serialunix serial;
Hash sha;

/**
 * Instantiate Commas and Commas_FACTORY
 */
Commas saife(&memfile, &serial, &sha);
Commas_FACTORY saife_factory(&memfile);

/**
 * Create the key.h header file which contains all of the keys
 * and signatures necessary for a Border Router and End Device
 */
int main(int argc, char const *argv[])
{
	saife.seedRandom(time(NULL));
	saife_factory.setPrivateKey(CAPrivateKey);

	// Create an output file stream
	ofstream file;
	file.open("keys.h");
	// Check to ensure the file is open
	if(file.is_open())
	{
		// Write Certificate Authority Keys to file
		file << "// CA Keys\n";
		file << "char CAPublicKey[] = \"";
		file << CAPublicKey;
		file << "\";\n";
		file << "char CAPrivateKey[] = \"";
		file << CAPrivateKey;
		file << "\";\n";

		// Write End Device Keys to file
		file << "// End Device Keys\n";
		file << "char EDPublicKey[] = \"";
		file << EDPublicKey;
		file << "\";\n";
		file << "char EDPrivateKey[] = \"";
		file << EDPrivateKey;
		file << "\";\n";
		file << "char EDUUID[] = \"";
		file << EDUUID;
		file << "\";\n";
		file << "char EDATTSIG[] = \"";

		// Sign the End Device Public Key with the CA Private Key
		saife_factory.setPublicKey(EDPublicKey);
		
		uint8_t EndDevicePublicKey[64];
		saife_factory.readMem(EndDevicePublicKey, 64, 2);

		uint8_t EndDeviceSignature[64];
		saife.sign(EndDevicePublicKey, EndDeviceSignature, sizeof(EndDevicePublicKey));

		char EndDeviceSignatureBuffer[129];

		for(uint8_t i = 0; i < sizeof(EndDeviceSignature); i++)
		{
			sprintf(&EndDeviceSignatureBuffer[2*i], "%02x", EndDeviceSignature[i]);
		}
		file << EndDeviceSignatureBuffer;
		file << "\";\n";

		// Write Border Router Keys to file
		file << "// Border Router Keys\n";
		file << "char BRPublicKey[] = \"";
		file << BRPublicKey;
		file << "\";\n";
		file << "char BRPrivateKey[] = \"";
		file << BRPrivateKey;
		file << "\";\n";
		file << "char BRUUID[] = \"";
		file << BRUUID;
		file << "\";\n";
		file << "char BRATTSIG[] = \"";

		// Sign the Border Router Public Key with the CA Private Key
		saife_factory.setPublicKey(BRPublicKey);
		
		uint8_t BorderRouterPublicKey[64];
		saife_factory.readMem(BorderRouterPublicKey, 64, 2);

		uint8_t BorderRouterSignature[64];
		saife.sign(BorderRouterPublicKey, BorderRouterSignature, sizeof(BorderRouterPublicKey));

		char BorderRouterSignatureBuffer[129] = {0};

		for(uint8_t i = 0; i < sizeof(BorderRouterSignature); i++)
		{
			sprintf(&BorderRouterSignatureBuffer[2*i], "%02x", BorderRouterSignature[i]);
		}
		file << BorderRouterSignatureBuffer;
		file << "\";\n";

		file.close();
	}
	// Fail cleaner if we're unable to open the file
	else
	{
		printf("Unable to open file\n");
	}
	return 0;
}