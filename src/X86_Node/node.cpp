#include "commas.h"
#include "file_mem.h"
#include "serialunix.h"
#include "shaunix.h"
#include "keys.h"
#include <time.h>
#include <stdio.h>
#include <unistd.h>

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
 * Set the relevant keys and run the state machine
 */
int main(int argc, char const *argv[])
{
	printf("END DEVICE STARTING\n");

	saife.seedRandom(time(NULL));

	saife_factory.setCA(CAPublicKey);
	saife_factory.setPublicKey(EDPublicKey);
	saife_factory.setPrivateKey(EDPrivateKey);
	saife_factory.setUUID(EDUUID);
	saife_factory.setAttestationSig(EDATTSIG);

	// Terrible, horrible state machine
	for(uint8_t i = 0; i < 3; i ++)
	{
		saife.sendHelloMessage();
		usleep(1);
	}

	for(;;)
	{
		if(saife.verifyJoinRequest())
		{
			for(uint8_t i = 0; i < 3; i ++)
			{
				saife.sendJoinResponse();
				usleep(1);
			}
			for(;;)
			{
				if(saife.verifyTokenSignature())
				{
					for(uint8_t i = 0; i < 3; i ++)
					{
						saife.sendACK();
						usleep(1);
					}
				}
			}
		}
		usleep(1);
	}
	return 0;
}