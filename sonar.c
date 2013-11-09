/*
 * sonar.c
 *
 * Created: 11/6/2013 2:12:06 PM
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include "serial.h"
#include <avr/interrupt.h>
#include <util/delay.h>

//Declare Variables to be handled by ISR
volatile unsigned int pulsewidth;
volatile unsigned int sonarIsPolling;

ISR(PCINT0_vect){
	//React to the pin change for the sonar
	
	//Stopping interrupts not necessary b.c interrupts stop before ISR
	unsigned int timervalue = TCNT1;	//record timer value to i
	TCNT1 = 0;							//reset timer
	//If falling edge of pulse
	if (!((PINB>>0)&1)){
		pulsewidth = timervalue;		//record timer value
		sonarIsPolling = 0;				//Reset polling flag
	}
}

void initSonar(void){
	//Initializes sonar interrupts and timer
	PCICR |= (1<<0);			//initializing pinchange interrupts
	PCMSK0 &= ~(1<<0);			
	sei();						//enable global interrupt
	TCCR1B |= (1<<1|1<<0);		//setting prescaler to 64
	TCCR1B &= ~(1<<2);
}

static inline void startSonarMeasurement(void){
	//Sends trigger pulse to start sonar measurement
	PCMSK0 &= ~(1<<0);			//disable interrupt
	DDRB |= (1<<0);				//set pin to output
	PORTB |= (1<<0);			//set pin to high
	_delay_us(5);				//delay 5 microsecs
	PORTB &= ~(1<<0);			//set pin to low
	DDRB &= ~(1<<0);			//set pin to input
	PCMSK0 |= (1<<0);			//enable interrupt
	sonarIsPolling = 1;			//set polling flag
}

float getSonar(void){
	//Return the most recent sonar value and make sure that the sonar is polling

	//If the sonar isn't polling, make it poll
	if (!sonarIsPolling){
		startSonarMeasurement();			//call the sonar measurement function
	}
	return ((float)pulsewidth*.02712);		//return the distance value in inches
}

//int main(void){
//	init_uart();
//	initSonar();
//	float distance;
//	while(1){
//		distance = getSonar();	//get the distance for the current block
//		printf("The distance is %u [inches] \r\n",(uint16_t)distance); //print distance
//		_delay_ms(10);			//delay corresponding to 100 Hz
//	}
//}
