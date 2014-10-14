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
	uint8_t state = 0;
	for(;;)
	{
		switch(state)
		{
			case 0:
				saife.sendHelloMessage();
				usleep(500000);
				state = 1;
				break;
			case 1:
				if(saife.verifyJoinRequest())
				{
					state = 2;
				}
				else
				{
					state = 0;
				}
				break;
			case 2:
				saife.sendJoinResponse();
				usleep(500000);
				state = 3;
				break;
			case 3:
				if(saife.verifyTokenSignature())
				{
					state = 4;
				}
				else
				{
					state = 2;
				}
				break;
			case 4:
				for(uint8_t i = 0; i < 2; i ++)
				{
					saife.sendACK();
					usleep(500000);
				}
				state = 5;
				break;
			case 5:
				static uint8_t data[] = { 'B', 'C', 'H'};
				saife.sendSignedMessage(data, sizeof(data));
				usleep(500000);
				break;
		}
	}
	return 0;
}