/* sonar.c
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

void motor_init(void){
// Function to initialize PWM

	// Set OCR0A when high, reset when low
	TCCR0A |= (1<<COM0A1)|(1<<COM0A0);
	TCCR2A |= (1<<COM2A1)|(1<<COM2A0);
	// Reset OCR0B when high, set when low
	TCCROA |= (1<<COMOB1);
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
}

void motor_setSpeed(uint8_t motornum, int_8t speed){
// Function to set motor speed
	switch (motornum){
	case 0:
		// Right Motor
		break;
	case 1:
		// Left Motor
		break;
	}
}
