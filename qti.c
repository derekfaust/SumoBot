/* qti.c
 * 
 * Module contains everything needed to operate a PING sonar sensor.
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

// Include standard headers
#include <avr/io.h>			// For general I/O
#include <avr/interrupt.h>	// For interrupts
#include <stdint.h>			// For data types

// Include local headers
#include "motor.h"			// Motor control to stop motors

/* Connection Details
 * Pin	Sensor Placement
 * ---------------------
 * PD0	Center
 * PD1	Front
 * PD2	Back
 */

//Define variables
static volatile uint8_t touchingBounds;

/* Begin Internal Functions Here
 * =============================
 *
 */

ISR(PCINT2_vect){
// ISR to detect out-of-bounds

	//If center QTI detects white 
	if ((PIND>>0)&1){
		_delay_us(500);
		//Check again after short wait
		if ((PIND>>0)&1){
			// Accept defeat
			
			// Turn Motors off
			motor_setSpeed(0,0);
			motor_setSpeed(0,1);

			//Initialize LED
			DDRB |= (1<<5);

			while (1){
				//Blink LED indefinitely
				PORTB ^= (1<<5);
				_delay_ms(250);
			}
		}
	}
	if ((PIND>>1)&1){
		// Set zeroth bit if front is touching
		touchingBounds |= (1<<0);
	}
	if ((PIND>>2)&1){
		// Set first bit if back is touching
		touchingBounds |= (1<<1);	
	}

}

/* Begin Global Functions Here
 * ===========================
 *
 */

void qti_init(void){
// Initialize all QTI sensors

	// Ensure that pins are set to input
	DDRD &= ~((1<<0)|(1<<1)|(1<<2));
	// Enable PCINT2
	PCICR |= (1<<2);
	// Allow first 3 pins to trigger interrupt
	PCMSK2 |= (1<<0)|(1<<1)|(1<<2);
	// Enable interrupts globally
	sei();
}

uint8_t qti_touchingBounds(void){
// Pass where robot is touching bounds to
// the main program loop
	return touchingBounds;
}