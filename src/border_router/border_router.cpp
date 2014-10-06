#include "commas.h"
#include "file_mem.h"
#include "serialunix.h"
#include "shaunix.h"
#include "keys.h"
#include <unistd.h>  /* UNIX standard function definitions */
#include <time.h>
#include <stdio.h>

/**
 * Intstantiate interface implementations
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
 * Set keys and run the border router side join sequence state machine
 */
int main(int argc, char const *argv[])
{
	printf("BORDER ROUTER STARTING\n");

	uint8_t dataBuffer[32];

	saife.seedRandom(time(NULL));

	saife_factory.setCA(CAPublicKey);
	saife_factory.setPublicKey(BRPublicKey);
	saife_factory.setPrivateKey(BRPrivateKey);
	saife_factory.setUUID(BRUUID);
	saife_factory.setAttestationSig(BRATTSIG);
	
	// We really need to fix these nested loops with a better state machine
	for(;;)
	{
	    if(saife.verifyHelloMessage())
	    {
	    	printf("Hello Message verified\n");
	        for(;;)
	        {
		        saife.sendJoinRequest();
		        if(saife.verifyJoinResponse())
		        {
		        	printf("\nBR join response verified");
		        	for(;;)
		        	{
		        		saife.sendTokenSignature();
		        		if(saife.verifyACK())
		        		{
		        			printf("\nACK VERIFIED!!!");
		        			for(;;)
		        			{
		        				memset(dataBuffer, 0, sizeof(dataBuffer));
		        				if(saife.verifySignedMessage(dataBuffer))
		        				{
		        					printf("Signed Data Verified!\nData: ");
		        					for(uint8_t i = 0; i < sizeof(dataBuffer); i++)
		        					{
		        						printf("%c", dataBuffer[i]);
		        					}
		        				}
		        			}
		        		}
		        	}
		        }
		    }
	    }
	}
	return 0;
}