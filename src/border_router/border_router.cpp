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
	
	// Border router join sequence state machine
	uint8_t state = 0;
	for(;;)
	{
		switch(state)
		{
			case 0:
	    		if(saife.verifyHelloMessage())
	    		{
	    			printf("Hello Message verified\n");
	    			state = 1;
	    		}
	    		break;
	    	case 1:
		        saife.sendJoinRequest();
		        state = 2;
		        break;
		    case 2:
		        if(saife.verifyJoinResponse())
		        {
		        	printf("\nBR join response verified");
		        	state = 3;
		        }
		        else
		        {
		        	state = 1;
		        }
		        break;
		    case 3:
		        saife.sendTokenSignature();
		        state = 4;
		        break;
		    case 4:
		        if(saife.verifyACK())
		        {
		        	printf("\nACK VERIFIED!!!");
		        	state = 5;
		        }
		        else
		        {
		        	state = 3;
		        }
		        break;
		    case 5:
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
	return 0;
}