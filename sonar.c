/* sonar.c
 * 
 * Module contains everything needed to operate a PING sonar sensor.
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

//Define Constants
#ifndef F_CPU
#define F_CPU 16000000UL		// CPU Clock Frequency
#endif
#define NUM_SONARS 2			// Number of sonars
#define DETECT_THRESHOLD 20		// Threshold for detecting an object
								// [1.73 inch increments]
#define MAX_PULSE 4625			// Maximum return pulse time

//Include standard headers
#include <avr/io.h>			// For general I/O
#include <avr/interrupt.h>	// For interrupts
#include <util/delay.h>		// For delay in output pulse
#include <stdint.h>			// For data types

//Include Local Headers
#include "sonar.h"			// Header for this file

//Define global variables
static volatile uint16_t distance[NUM_SONARS];	// Record raw timer values for sonars
static uint8_t dist8[NUM_SONARS];				// 8-bit distances values for sonars
static uint8_t newDist;							// Flags indicating a new measurement
static volatile uint8_t pulseStarted;			// Record if input pulse has started
static volatile uint8_t pulseReceived;			// Record how many pulses are recieved
static uint8_t pinnum[] = {0,1};				// Specify the pin of each sonar


/* Begin Internal Functions Here
 * =============================
 *
 */

ISR(PCINT0_vect){
// ISR to record the value of the signal
// Hard-coded to use PCINT0 and TIMER1
	
	//Stopping interrupts not necessary b.c interrupts stop before ISR
	uint16_t timervalue = TCNT1;	//Record timer value

	// If waiting for pulse to end and pin is low, bit is 1
	uint8_t pulseEnded = pulseStarted & (~PINB);
	
	/* Note: If the rising edge of the second sensor is caught,
	 * neither line will be low and pulseEnded will be 0
	 */
	if (pulseStarted){
		// If waiting for a sensor
		if ((pulseEnded>>pinnum[0])&1){			// If first sonar pulse ends
			distance[0] = timervalue;			// Record Timer Value
			pulseStarted &= ~(1<<pinnum[0]);	// No Longer Polling this Sonar
			pulseReceived++;					// Note that the pulse was received
			newDist |= (1<<0);					// Note a new measurement

		}else if ((pulseEnded>>pinnum[1])&1){	// If second sonar pulse ends
			distance[1] = timervalue;			// Record Timer Value
			pulseStarted &= ~(1<<pinnum[1]);	// No Longer Polling this Sonar
			pulseReceived++;					// Note that the pulse was received
			newDist |= (1<<1);					// Note a new measurement
		}

	}else{
		// Reset the timer
		TCNT1 = 0;
		// Set the pulse as started on both sonars
		pulseStarted = ((1<<pinnum[0])|(1<<pinnum[1]));
		pulseReceived=0;
	}
}

uint16_t getTimer(void){
// Function to read the timer value

	uint8_t sreg;	// Initialize variables
	uint16_t time;
	sreg = SREG;	// Store interrupt register state
	cli();			// Disable interrupts
	time = TCNT1;	// Read timer value
	SREG = sreg;	// Restore interrupt states
	return time;	// Return timer value
}

void pollSonar(void){
// Polls the next sensor if none are polling

	if ((pulseReceived == NUM_SONARS)||(getTimer()>MAX_PULSE)){
		// If no sonars are polling or if time has been exceeded

		//Send trigger pulse to start sonar measurement on multiple sonars
		//Disable interrupt
		PCMSK0 &= ~((1<<pinnum[0])|(1<<pinnum[1]));
		//Set pin to output
		DDRB |= ((1<<pinnum[0])|(1<<pinnum[1]));
		//Set pin to high
		PORTB |= ((1<<pinnum[0])|(1<<pinnum[1]));
		//Delay 5 microsecs
		_delay_us(5);
		//Set pin to low
		PORTB &= ~((1<<pinnum[0])|(1<<pinnum[1]));
		//Set pin to input
		DDRB &= ~((1<<pinnum[0])|(1<<pinnum[1]));
		//Enable interrupt
		PCMSK0 |= ((1<<pinnum[0])|(1<<pinnum[1]));
	}
}

void distToByte(void){
// Converts 16-bit distances to 8-bit distances

	uint8_t iter_sonar;		// Initialize iterator
	//For every sensor, convert it;s distance to one byte
	for (iter_sonar=0;iter_sonar<NUM_SONARS;iter_sonar++){
		// Divide by 32 to convert to one byte.
		dist8[iter_sonar] = (uint8_t)(distance[iter_sonar]>>5);
	}
}


/* Begin Global Functions Here
 * ===========================
 *
 */

void sonar_init(void){
//Initializes sonar interrupts and timer
	
	//Initialize PCINT0
	PCICR |= (1<<0);
	//Enable interrupts globally
	sei();
	//Set Prescaler to 64
	TCCR1B |= (1<<1|1<<0); TCCR1B &= ~(1<<2);
}

uint8_t sonar_isNewDist(int8_t direction){
// Returns true if there is a new distance measurement in a given direction.
	uint8_t newFlag;
	switch (direction) {
	case -1:
		// If direction is behind, return back sensor value
		newFlag = ((newDist >> 1) & 1);	// Save Value
		newDist &= ~(1<<1);				// Reset Value
		return newFlag;					// Return Value
		break;
	case 1:
		// If direction is forward, return front sensor value
		newFlag = ((newDist >> 0) & 1);	// Save Value
		newDist &= ~(1<<0);				// Reset Value
		return newFlag;					// Return Value
		break;
	default:
		// If direction is not specified, either will work.
		newFlag = newDist;				// Save Value
		newDist &= ~((1<<0)|(1<<1));	// Reset Flags
		return newFlag;					// Return Value
		break;
	}

}

uint8_t sonar_getDistance(uint8_t sonarnum){
// Trigger polling and return distance of object
/* Distance is returned as a value from 0 to 145
 * with a resolution of 1.73 inches.
 */
	
	//Trigger sonar poll
	pollSonar();

	// Return the appropriate value as requested by the function call.
	if(sonarnum>=0 && sonarnum<=NUM_SONARS){
		// If distance requested from a specific sensor,
		// then return the distance from that sensor

		// Return the value divided by 32 so it fits in one byte
		/* Note: typically, distToByte() should be called,
		 * but this is faster if only one value is needed
		 */
		return (uint8_t)(distance[sonarnum]>>5);

	}else{
		// Otherwise, return the smallest value
		// Note: this function is significantly slower as
		// 16-bit variables must be compared.

		uint8_t iter_sonar;				// Initialize iteration variable
		uint8_t minDist = dist8[0];	// Initialize compare variable
		
		// Compare all sonar distances
		for(iter_sonar=0;iter_sonar<NUM_SONARS;iter_sonar++){
			if (dist8[iter_sonar]<minDist){
				// If distance is less than shortest distance,
				// then it is the shortest distance.
				minDist = dist8[iter_sonar];
			}
		}

		// Return the value divided by 32 so it fits in one byte
		return minDist;
	}
}


int8_t sonar_getRegion(void){
// Return the region that an object is detected in
	
	// Trigger sonar poll
	pollSonar();

	// Convert raw values to bytes
	distToByte();

	// Map which sonars detected an object
	uint8_t detectMap=0;	// Map of hits
	uint8_t i;				// Iteration variable
	for(i=0;i<NUM_SONARS;i++){
		if(dist8[i]<DETECT_THRESHOLD){
			// Set bit if sonar detected something
			detectMap |= (1<<i);
		}
	}

	// Translate hits on individual sensors to a region
	int8_t region=0;
	switch (detectMap){
		case (1<<0):
			// Front Detect
			region = 1;
			break;
		case (1<<1):
			// Back Detect
			region = -1;
			break;
		case (1<<1)|(1<<0):
			//Front and back Detect
			region = 0;
			break;
	}
	return region;
}
