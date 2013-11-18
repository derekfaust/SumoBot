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
#define HALF_PERIOD 13		// Half the period of the pulse
#define PULSE_LENGTH 100	// Number of periods to pulse

//Include standard libraries
#include <stdint.h>			// For standard data types	
#include <avr/io.h>			// For general I/O
#include <util/delay.h>		// For delays

// Include local modules
#include "infrared.h"		// Include own header file

// Define array of half-periods (closest range first)
static uint8_t activeMap;
static uint8_t passiveMap;

/* Begin Internal Functions Here
 * =============================
 *
 */

void pollInfrared(uint8_t active){

	if(active){
		// Pulse the output port
		
		uint8_t iter_pulse; 			// Initialize iteration variable

		// Loop for the number of periods in the pulse
		for (iter_pulse = 0; iter_pulse<PULSE_LENGTH; iter_pulse++){
			PORTD |= (1<<7);				// Turn on the LEDs
			_delay_us(HALF_PERIOD);			// Wait half the period
			PORTD &= ~(1<<7);				// Turn off the LEDs
			_delay_us(HALF_PERIOD);			// Wait half the period
		}
			
		// Record the received pulses from
		// the first 6 registers in port c
		activeMap = (~PINC)|0x01;
	}else{
		// Record which sensors are recieving
		// Stray IR signals
		passiveMap = (~PINC)|0x01;
	}
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

uint8_t infrared_activeMap(void){
// Function to get a map locating where
// around the periphery object are detected.
	pollInfrared(1);
	return activeMap;
}

uint8_t infrared_passiveMap(void){
// Function to return a map of where
// external IR signals are being detected
	pollInfrared(0);
	return passiveMap;
}
