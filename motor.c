/* motor.c
 * 
 * Module contains everything needed to operate an H-bridge.
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

//Include standard headers
#include <avr/io.h>			// For general I/O
#include <avr/interrupt.h>	// For interrupts
#include <stdint.h>			// For data types

//Include Local Headers
#include "serial.h"			// For serial terminal readout
#include "motor.h"			// Header for this file

//Definitions
#define DEAD_TIME 1			// Define PWM dead time

/* Connection details
 * Pin		Side	Direction	
 * --------------------------
 * OCR0A	Left	Forward
 * OCR0B	Left	Reverse
 * OCR2A	Right	Forward
 * OCR2B	Right	Reverse
 */

//Define variables
static uint8_t rightSpeed[]={0,42,87,127,167,213,255};
static uint8_t leftSpeed[]={0,42,87,127,167,213,255};

void motor_init(void){
// Function to initialize PWM

	// Set OCR0A when high, reset when low
	TCCR0A |= (1<<COM0A1)|(1<<COM0A0);
	TCCR2A |= (1<<COM2A1)|(1<<COM2A0);
	// Reset OCR0B when high, set when low
	TCCR0A |= (1<<COM0B1);
	TCCR2A |= (1<<COM2B1);
	/* This is set up such that OCRnB should always be lower
	 * than OCRnA to allow time for the transistors to switch
	 */

	//Set Initial Speed
	motor_setSpeed(0, 0);
	motor_setSpeed(1, 0);

	// Set prescalar to 8
	TCCR0B |= (1<<CS01);
	TCCR2B |= (1<<CS21);
	
	// Set Waveform Generation Mode to Phase-correct PWM
	TCCROA |= (1<<WGM00);
	TCCR2A |= (1<<WGM20);

	// Set motor pins to outputs
	PORTD |= (1<<3)|(1<<5)|(1<<6);
	PORTB |= (1<<3);
}

void motor_setSpeed(uint8_t motornum, int8_t speed){
// Function to set motor speed
	if(speed >= -3 && speed <= 3){
		uint8_t setpt;
		switch (motornum){
		case 0:
			// Left Motor
			
			// Find the setpt from the array
			setpt = leftSpeed[speed+3];
			
			if(setpt > 0xFF-DEAD_TIME){
				// If setpt would overflow on the high side
				// Then move setpt to max value
				setpt = 0xFF-DEAD_TIME;
			}else if(setpt < DEAD_TIME){
				// If setpt would overflow on the low side
				// Then move setpt to min value
				setpt = DEAD_TIME;
			}
			
			//Set the motor speed
			OCR0A = setpt+DEAD_TIME;
			OCR0B = setpt-DEAD_TIME;
			break;
		case 1:
			// Right Motor
			
			// Find the setpt from the speed array
			setpt = rightSpeed[speed+3];

			if(setpt > 0xFF-DEAD_TIME){
				// If setpt would overflow on the high side
				// Then move setpt to max value
				setpt = 0xFF-DEAD_TIME;
			}else if(setpt < DEAD_TIME){
				// If setpt would overflow on the low side
				// Then move setpt to min value
				setpt = DEAD_TIME;
			}
			
			//Set the motor speed
			OCR2A = setpt+DEAD_TIME;
			OCR2B = setpt-DEAD_TIME;
			break;
		}
	}
}
