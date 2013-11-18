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

// Define Connections
#define IR_OUT 4

/* Connection Details
 * Pin	Sensor Placement
 * ---------------------
 * PC4	IR Output
 * PC0	Front Left	
 * PC1	Front Right
 * PC2	Back Left
 * PC3	Back Right
 */


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
			PORTC |= (1<<IR_OUT);			// Turn on the LEDs
			_delay_us(HALF_PERIOD);			// Wait half the period
			PORTC &= ~(1<<IR_OUT);			// Turn off the LEDs
			_delay_us(HALF_PERIOD);			// Wait half the period
		}
			
		// Record the received pulses from
		// the first 4 registers in port c
		activeMap = (~PINC)|0x0F;
	}else{
		// Record which sensors are recieving
		// Stray IR signals
		passiveMap = (~PINC)|0x0F;
	}
}

/* Begin Global Functions Here
 * ===========================
 *
 */

void infrared_init(){
// Function to initialize all IR sensors.
	
	DDRC |= (1<<IR_OUT);	// Set IR output pin to output
	DDRC &= 0xF0;			// Set PC0-5 to input
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
