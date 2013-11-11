/* test_sonar.h
 * 
 * Program to test the sonar module sonar.c
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

//Define Constants
#ifndef F_CPU
#define F_CPU 16000000UL		// CPU Clock Frequency
#endif

// Include standard libraries
#include <avr/io.h>		// For general I/O
#include <util/delay.h>	// For delay
#include <stdint.h>		// For data types

// Include local libraries
#include "sonar.h"		// Library to be tested
#include "serial.h"		// Library for serial terminal readout

int main(void)
{
    sonar_init();		// Initialize Sonar
	uint16_t distance; 	// Initialize distance variable
	
	// Loop forever
	while(1){
		// Get distance
        distance = sonar_getDistance(0);
		// Print the distance
		printf("The measured distance is %u", distance);
		// Wait a little
		_delay_ms(100);
    }
}
