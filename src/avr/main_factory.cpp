#include "avr/io.h"
#include "util/delay.h"
#include "eeprom_mem.h"
#include "commas.h"
#include "keys.h"

/**
 * Instantiate interface implementations
 * All we need for a Commas_FACTORY is the memory interface implementation
 */
eeprom_mem onBoardEEPROM;

/**
 * Instantiate a Commas_FACTORY
 */
Commas_FACTORY saife_factory(&onBoardEEPROM);

extern "C" void __cxa_pure_virtual() {
  for(;;);
}

/**
 * Set keys
 */
int main(void)
{
	saife_factory.setCA(CAPublicKey);
	saife_factory.setPublicKey(EDPublicKey);
	saife_factory.setPrivateKey(EDPrivateKey);
	saife_factory.setUUID(EDUUID);
	saife_factory.setAttestationSig(EDATTSIG);

	return 0;
}