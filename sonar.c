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
#define NUM_SONARS 3			// Number of sonars
#define DETECT_THRESHOLD 1327	// Threshold for detecting an object
#define MAX_PULSE 160000		// Maximum return pulse time

//Include standard headers
#include <avr/io.h>			// For general I/O
#include <avr/interrupt.h>	// For interrupts
#include <util/delay.h>		// For delay in output pulse
#include <stdint.h>			// For data types

//Include Local Headers
#include "sonar.h"			// Header for this file

//Define global variables
static volatile int8_t pollingSonar = -1;		// Notes which sonar is polling
static volatile int8_t lastPolled = -1;			// Notes the last sonar to poll
static volatile uint16_t distance[NUM_SONARS];	// Record distances sonars
static uint8_t pinnum[] = {0,1,2};				// Specify the pin of each sonar


/* Begin Internal Functions Here
 * =============================
 *
 */

ISR(PCINT0_vect){
// ISR to record the value of the signal
// Hard-coded to use PCINT0 and TIMER1
	
	//Stopping interrupts not necessary b.c interrupts stop before ISR
	unsigned int timervalue = TCNT1;	//Record timer value to i
	TCNT1 = 0;							//Reset timer
	//If falling edge of pulse
	if (!((PINB>>pinnum[pollingSonar])&1)){
		// Record Timer Value
		distance[pollingSonar] = timervalue;
		
		lastPolled = pollingSonar;
		//Reset polling flag
		pollingSonar=-1;
	}
}

uint16_t getTimer(void){
	uint8_t sreg;
	uint16_t time;
	sreg = SREG;
	cli();
	time = TCNT1;
	SREG = sreg;
	return time;
}

void pollSonar(void){
// Polls the next sensor if none are polling
	
	if (pollingSonar == -1 || (getTimer()>1500)){
		// If no sonars are polling

		if ((lastPolled+1) < NUM_SONARS){
			// If count doesn't need to be reset,
			// poll next sonar
			pollingSonar = lastPolled+1;
		}else{
			// Otherwise, poll first sensor
			pollingSonar = 0;
		}
		
		//Sends trigger pulse to start sonar measurement
		//Disable interrupt
		PCMSK0 &= ~(1<<pinnum[pollingSonar]);
		//Set pin to output
		DDRB |= (1<<pinnum[pollingSonar]);
		//Set pin to high
		PORTB |= (1<<pinnum[pollingSonar]);
		//Delay 5 microsecs
		_delay_us(5);
		//Set pin to low
		PORTB &= ~(1<<pinnum[pollingSonar]);
		//Set pin to input
		DDRB &= ~(1<<pinnum[pollingSonar]);
		//Enable interrupt
		PCMSK0 |= (1<<pinnum[pollingSonar]);
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


uint16_t sonar_getDistance(uint8_t sonarnum){
// Trigger polling and return distance of object
	
	//Trigger sonar poll
	pollSonar();

	// Return the appropriate value as requested by the function call.
	if(sonarnum>=0 && sonarnum<=NUM_SONARS){
		// If distance requested from a specific sensor,
		// then return the distance from that sensor
		return distance[sonarnum];
	}else{
		// Otherwise, find the average of all sensors that detect and object

		// Find number of sensors that detected something
		uint32_t sum = 0;		// Initialize sum of the distances to zero
		uint8_t numDetect = 0;	// Number of sonars detecting something
		uint8_t i;				// Iteration variable
		for(i=0;i<NUM_SONARS;i++){
			if(distance[i]<DETECT_THRESHOLD){
				// If object is detected by a given sensor:
				numDetect++;		//Increment number detected
				sum += distance[i];	//Add distance to the sum
			}
		}

		// Calculate and return average
		if(numDetect == 0 || numDetect == 1){
			// If zero or one sensors detect something, return sum
			return (uint16_t) sum;
		}else if(numDetect==2){
			// Division by two can be represented by a bitshift
			return (uint16_t) (sum>>1);
		}else{
			// Otherwise, do the division
			return (uint16_t) (sum/numDetect);
		}
	}
}


int8_t sonar_getRegion(){
// Return the region that an object is detected in
	
	//Trigger sonar poll
	pollSonar();

	// Map which sonars detected an object
	uint8_t detectMap=0;	// Map of hits
	uint8_t i;			// Iteration variable
	for(i=0;i<NUM_SONARS;i++){
		if(distance[i]<DETECT_THRESHOLD){
			// Set bit if sonar detected something
			detectMap |= (1<<i);
		}
	}

	// Translate hits on individual sensors to a region
	/* This block of code assumes that there are three sonars
	 * such that sonar0 is in the center, sonar 1 is left, and
	 * sonar 2 is right.
	 */
	uint8_t region=-8;
	switch (detectMap){
		case (1<<0):
			// Center Detect
			region = 0;
			break;
		case (1<<2):
			// Right Detect
			region = 2;
			break;
		case (1<<1):
			// Left Detect
			region = -2;
			break;
		case (1<<2)|(1<<0):
			//Center Right Detect
			region = 1;
			break;
		case (1<<1)|(1<<0):
			//Center Left Detect
			region = -1;
			break;
		case (1<<0)|(1<<1)|(1<<2):
			//All Detect
			region = 8;
			break;
	}
	return region;
}


void sonar_test(void){
    sonar_init();		// Initialize Sonar
	uint16_t distance; 	// Initialize distance variable
	DDRB |= (1<<5);
	
	// Loop forever
	while(1){
		// Get distance
        distance = sonar_getDistance(0);
		
		// Turn LED on for a proportional amount of time
		PORTB |= (1<<5);
		for (int i=0; i<distance; i++){
			_delay_ms(1);
		}
		PORTB &= ~(1<<5);
		
		// Wait a little
		_delay_ms(100);
    }
}
