#ifndef
#define SONAR_H

//Necessary Includes
#include <stdint.h>

//Object Declarations
typedef struct{
	volatile uint8_t * port;
	volatile uint8_t * ddr;
	volatile uint8_t * pin;
	uint8_t pinn;
	volatile uint16_t pulsewidth;
	volatile uint8_t isPolling;
} SONAR;

SONAR initSonar(
	volatile uint8_t * port,
	volatile uint8_t * ddr,
	volatile uint8_t * pin,
	uint8_t pinn
	);

#endif
