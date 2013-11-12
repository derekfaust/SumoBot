/* infrared.c
 * 
 * Module contains everything needed to operate the infrared sensors
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

// Define Constants
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#define PULSE_LENGTH 100	// Pulse length in # of periods
#define IR_HALF_PERIOD 13	// Half the period in us

//Include standard libraries
#include <stdint.h>			// For standard data types	
#include <avr/io.h>			// For general I/O
#include <util/delay.h>		// For delays

// Include local modules
#include "infrared.h"		// Include own header file

/* Begin Internal Functions Here
 * =============================
 *
 */

uint8_t pollInfrared(void){
// Poll all of the infrared sensors
	
	//Initialize map variable
	uint8_t map;
	
	// Pulse the output port
	uint8_t iterpulse; 			// Initialize iteration variable

	// Loop for the number of periods in the pulse
	for (iter_pulse = 0; iter_pulse<PULSE_LENGTH; iter_pulse++){
		PORTC |= (1<<7);				// Turn on the LEDs
		_delay_us (IR_HALF_PERIOD) ;	// Wait half the period
		PORTC &= ~(1<<7);				// Turn off the LEDs
		_delay_us (IR_HALF_PERIOD);		// Wait half the period
	}
	
	// Record the received pulses from
	// the first 6 registers in port c
	map = PINC|(~((1<<6)|(1<<7))); 

	// Return the results
	return map;
}

/* Begin Global Functions Here
 * ===========================
 *
 */

void infrared_init(){
// Function to initialize all IR sensors.
	
	DDRD |= (1<<7);				// Set PD7 to output
	DDRC &= ~((1<<7)|(1<<6));	// Set PC0-5 to input
}

int8_t infrared_getMap(){
// Function to get a map locating where
// around the periphery object are detected.
	uint8_t map = pollInfrared();
	return map;
}
