/* sonar.c
 * 
 * Module contains everything needed to operate a PING sonar sensor.
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */


//Include standard headers
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

//Include Local Headers
#include "serial.h"
#include "sonar.h"

//Define Constants
#define F_CPU 16000000UL
#define NUM_SONARS 3


//Define global variables
static volatile int8_t pollingSonar = -1;
static volatile int8_t lastPolled = -1;
static volatile uint16_t distance[NUM_SONARS]
static uint8_t pinnum[] = {0,1,2};

/* ISR to record the value of the signal
 * Hard-coded to use PCINT0 and TIMER1
 */
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

void initSonar(void);
	
	//Initializes sonar interrupts and timer
	PCICR |= (1<<0);			//Initialize PCINT0
	PCMSK0 &= ~(1<<pinn);		//Initialize specific pin for PCINT0
}


void initAllSonars(void){
	sonar0 = initSonar(&PORTB, &DDRB, &PINB, 0);	//Initialize each sonar
	sonars[1] = {sonar0};							//Create array of sonar objects
	sei();											//Enable interrupts globally
	TCCR1B |= (1<<1|1<<0); TCCR1B &= ~(1<<2);		//Set prescaler to 64
	}

void startSonarMeasurement(void){
	if (pollingSonar == -1){
		if ((lastPolled+1) < NUMSONARS){
			pollingSonar = lastPolled+1;
		}else{
			pollingSonar = 0;
		}

		//Sends trigger pulse to start sonar measurement
		//disable interrupt
		PCMSK0 &= ~(1<<pinnnum[pollingSonar]);
		//set pin to output
		DDRB |= (1<<pinnum[pollingSonar]);
		//set pin to high
		PORTB |= (1<<pinnum[pollingSonar]);
		//delay 5 microsecs
		_delay_us(5);
		//set pin to low
		PORTB &= ~(1<<pinnum[pollingSonar]);
		//set pin to input
		DDRB &= ~(1<<pinnum[pollingSonar]);
		//enable interrupt
		PCMSK0 |= (1<<pinnum[pollingSonar]);
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
