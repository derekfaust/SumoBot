/* qti.c
 * 
 * Module contains everything needed to operate a PING sonar sensor.
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// Include standard headers
#include <avr/io.h>			// For general I/O
#include <util/delay.h>		// For delay
#include <avr/interrupt.h>	// For interrupts
#include <stdint.h>			// For data types

// Include local headers
#include "motor.h"			// Motor control to stop motors
#include "indicator.h"		// Indicator control
#include "qti.h"			// Own header file

// Define Connections
#define CENTER_QTI 2
#define FRONT_QTI 4
#define BACK_QTI 7

/* Connection Details
 * Pin	Sensor Placement
 * ---------------------
 * PD2	Center
 * PD4	Front
 * PD7	Back
 */

// Variable definitions
volatile int8_t qti_touchingBounds=0;

/* Begin Internal Functions Here
 * =============================
 *
 */

ISR(PCINT2_vect){
// ISR to detect out-of-bounds

	//If center QTI detects white 
	if ((~PIND>>CENTER_QTI)&1){
		_delay_us(500);
		//Check again after short wait
		if ((~PIND>>CENTER_QTI)&1){
			// Accept defeat
			
			// Turn Motors off
			motor_setSpeed(0,0);

			//while (1){
				//Blink LED indefinitely
				//indicator_redFlash(250);
				//_delay_ms(250);
			//}
		}
	}
	qti_touchingBounds = 0;
	if ((~PIND>>FRONT_QTI)&1){
		// Set to 1 if front is touching
		qti_touchingBounds = 1;
	}else if ((~PIND>>BACK_QTI)&1){
		// Set to -1 if back is touching
		qti_touchingBounds = -1;	
	}
}

/* Begin Global Functions Here
 * ===========================
 *
 */

void qti_init(void){
// Initialize all QTI sensors

	// Ensure that pins are set to input
	DDRD &= ~((1<<CENTER_QTI)|(1<<FRONT_QTI)|(1<<BACK_QTI));
	// Enable PCINT2
	PCICR |= (1<<CENTER_QTI);
	// Allow first 3 pins to trigger interrupt
	PCMSK2 |= ((1<<CENTER_QTI)|(1<<FRONT_QTI)|(1<<BACK_QTI));
	// Enable interrupts globally
	sei();
}
