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
int8_t motor_currentSpeed[2]={0,0};
int8_t motor_dirTurn=0;
int8_t motor_dirTravel=0;


/* Begin Global Functions here
 * ==========================
 *
 */

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

	// Set prescalar to 8
	TCCR0B |= (1<<CS01);
	TCCR2B |= (1<<CS21);
	
	// Set Waveform Generation Mode to Phase-correct PWM
	TCCR0A |= (1<<WGM00);
	TCCR2A |= (1<<WGM20);

	// Set motor pins to outputs
	DDRD |= (1<<3)|(1<<5)|(1<<6);
	DDRB |= (1<<3);
}

void motor_setSpeed(int8_t speed0, int8_t speed1){
// Function to set motor speed
	// Left Motor

	// Find the setpt from the array
	uint8_t setpt0 = leftSpeed[speed0+3];
	uint8_t setpt1 = rightSpeed[speed1+3];
	
	// Check to make sure register won't overflow
	if(setpt0 > 0xFF-DEAD_TIME){
		// If setpt would overflow on the high side
		// Then move setpt to max value
		setpt0 = 0xFF-DEAD_TIME;
	}else if(setpt0 < DEAD_TIME){
		// If setpt would overflow on the low side
		// Then move setpt to min value
		setpt0 = DEAD_TIME;
	}
	
	if(setpt1 > 0xFF-DEAD_TIME){
		// If setpt would overflow on the high side
		// Then move setpt to max value
		setpt1 = 0xFF-DEAD_TIME;
	}else if(setpt1 < DEAD_TIME){
		// If setpt would overflow on the low side
		// Then move setpt to min value
		setpt1 = DEAD_TIME;
	}
	
	// Set the motor speed
	OCR0A = setpt0+DEAD_TIME;
	OCR0B = setpt0-DEAD_TIME;
	
	OCR2A = setpt1+DEAD_TIME;
	OCR2B = setpt1-DEAD_TIME;
	
	// Record the speed values
	motor_currentSpeed[0] = speed0;
	motor_currentSpeed[1] = speed1;

	// Record direction
	motor_dirTurn = 0;
	motor_dirTravel = 0;
	int8_t diff = speed0-speed1;
	int8_t sum = speed0+speed1;
	if (diff<0){
		// If left is less than right
		// Turning Left
		motor_dirTurn = -1;
	}else if(diff>0){
		// If left is more than right
		// Turning Right
		motor_dirTurn = 1;
	}
	if(sum>0){
		// If combined is more than zero
		// Moving forward
		motor_dirTravel = 1;
	}else if(sum<0){
		// If combined is less than zero
		// Moving Backwards
		motor_dirTravel = -1;
	}
}
