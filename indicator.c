/* indicator.c
 * 
 * Module contains functions to manipulate LEDs, buzzers, etc
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

// Define Constants
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#define BEEP_HALF_PERIOD 13		// Half the period of the buzzer
#define BEEP_LENGTH 100			// Number of periods to pulse buzzer
								// to make a crisp beep

// Interface pins
#define GREEN_PIN 4				// PB# for green LED
#define RED_PIN 5				// PB# for red LED
#define BUZZER_PIN 4			// PD# for Buzzer

//Include standard libraries
#include <stdint.h>			// For standard data types	
#include <avr/io.h>			// For general I/O
#include <util/delay.h>		// For delays


/* Begin Global Functions Here
 * ===========================
 *
 */

// Function to initialize indicator pins
void indicator_init(void){
	// Set buzzer pin to output
	DDRD |= (1<<BUZZER_PIN);
	// Set LED pins to outputs
	DDRB |= (1<<RED_PIN)|(1<<GREEN_PIN);
}

// Function to make the buzzer beep
void indicator_beep(void){
	// Initialize iterators
	uint8_t i_beep;	
	uint8_t i_period;

	for(i_beep=0;i_beep<BEEP_LENGTH;i_beep++){
		// For the length of the beep
		PORTD ^= (1<<BUZZER_PIN);		// Toggle the state of the pin
		for(i_period=0;i_period<BEEP_HALF_PERIOD;i_period++){
			_delay_us(1);	// Delay for half the period
		}
	}
	PORTD &= ~(1<<BUZZER_PIN);	//Ensure that the pin ends low
}

// Function to flash the green LED
void indicator_greenFlash(uint8_t time){
	PORTB |= (1<<GREEN_PIN);	// Turn LED on

	uint8_t i_time;		// Initialize iterator
	for(i_time=0; i_time<time; i_time++){
		_delay_ms(4);			// Wait the desired amount of time
	}
	PORTB &= ~(1<<GREEN_PIN);	// Turn LED off
}

// Function to flash the red LED
void indicator_redFlash(uint8_t time){
	PORTB |= (1<<RED_PIN);		// Turn LED on

	uint8_t i_time;		// Initialize iterator
	for(i_time=0; i_time<time; i_time++){
		_delay_ms(4);			// Wait the desired amount of time
	}

	PORTB &= ~(1<<RED_PIN);		// Turn LED off
}

// Function to set the green LED
void indicator_greenSet(uint8_t state){
	if(state){
		// If state is 1, turn the LED on
		PORTB |= (1<<GREEN_PIN);
	}else{
		// If state is 0, turn the LED off
		PORTB &= ~(1<<GREEN_PIN);
	}
}

// Function to set the red LED
void indicator_redSet(uint8_t state){
	if(state){
		// If state is 1, turn the LED on
		PORTB |= (1<<RED_PIN);
	}else{
		// If state is 0, turn the LED off
		PORTB &= ~(1<<RED_PIN);
	}
}
