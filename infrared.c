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
static uint8_t half_period[]={13,6};
static uint8_t zone_dist[]={12,6}
static uint8_t bin_map[NUM_ZONES];
static uint8_t ir_hist[NUM_ZONES][NUM_IR];
static uint8_t distance[NUM_IR];

/* Begin Internal Functions Here
 * =============================
 *
 */

void pollInfrared(void){
// Poll all of the infrared sensors in the given zone
	uint8_t iter_zone;
	for (iter_zone=0;iter_zone<NUM_ZONES; iter_zone++){

		// Pulse the output port
		uint8_t iter_pulse; 			// Initialize iteration variable
		uint8_t iter_period;

		// Loop for the number of periods in the pulse
		for (iter_pulse = 0; iter_pulse<PULSE_LENGTH; iter_pulse++){
			PORTD |= (1<<7);				// Turn on the LEDs
			for (iter_period = 0; iter_period<half_period[iter_zone]; iter_period++){
				_delay_us (1);				// Wait half the period
			}
			PORTD &= ~(1<<7);				// Turn off the LEDs
			for (iter_period = 0; iter_period<half_period[iter_zone]; iter_period++){
				_delay_us (1);				// Wait half the period
			}
		}
		
		// Record the received pulses from
		// the first 6 registers in port c
		bin_map[iter_zone] = (~PINC)|0x3F;
	}
}

void calcDistance(void){
	uint8_t i_zone;
	uint8_t i_sensor;
	float momentum;
	uint8_t measurement;
	for(i_zone=0;i_zone<NUM_ZONES;i_zone++){
		for(i_sensor=0;i_sensor<NUM_IR;i_sensor++){
			if ((bin_map[i_zone]>>i_sensor)&1){
				measurement = zone_dist[i_zone]*(1-SMOOTHING_FACTOR);
			}else{
				measurement = 0;
			}
			momentum = ir_hist[i_zone][i_sensor]*SMOOTHING_FACTOR;
			ir_hist[i_zone][i_sensor] = momentum + measurement;
		}
	}
	for(i_sensor=0;i_sensor<NUM_IR;i_sensor++){
		
		if((ir_hist[NUM_ZONES-1]+(ir_hist[NUM_ZONES-1]>>3))>=zone_dist[NUM_ZONES-1]){
			distance[i_sensor] = zone_dist[NUM_ZONES-1];
		}
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

int8_t infrared_getMap(){
// Function to get a map locating where
// around the periphery object are detected.
	uint8_t map = pollInfrared(0);
	return map;
}
