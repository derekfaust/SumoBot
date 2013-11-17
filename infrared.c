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
#define NUM_ZONES 2			// Number of zones
#define NUM_IR 6			// Number of IR sensors
#define SMOOTHING_FACTOR .7	// Smoothing factor

//Include standard libraries
#include <stdint.h>			// For standard data types	
#include <avr/io.h>			// For general I/O
#include <util/delay.h>		// For delays

// Include local modules
#include "infrared.h"		// Include own header file

// Define array of half-periods (closest range first)
static uint8_t half_period=13;
static uint8_t bin_map;

/* Begin Internal Functions Here
 * =============================
 *
 */

void pollInfrared(void){
	// Pulse the output port
	uint8_t iter_pulse; 			// Initialize iteration variable
	uint8_t iter_period;

	// Loop for the number of periods in the pulse
	for (iter_pulse = 0; iter_pulse<PULSE_LENGTH; iter_pulse++){
		PORTD |= (1<<7);				// Turn on the LEDs
		for (iter_period = 0; iter_period<half_period; iter_period++){
			_delay_us (1);				// Wait half the period
		}
		PORTD &= ~(1<<7);				// Turn off the LEDs
		for (iter_period = 0; iter_period<half_period; iter_period++){
			_delay_us (1);				// Wait half the period
		}
	}
		
	// Record the received pulses from
	// the first 6 registers in port c
	bin_map = (~PINC)|0x3F;
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

int8_t infrared_getMap(void){
// Function to get a map locating where
// around the periphery object are detected.
	pollInfrared();
	return bin_map;
}
