/* sonar.c
 * 
 * Module contains everything needed to operate a PING sonar sensor.
 *
 * MAE3780 Mechatronics Final Project
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


//Define global variables
static volatile int8_t pollingSonar = -1;

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

SONAR initSonar(
	volatile uint8_t * port,
	volatile uint8_t * ddr,
	volatile uint8_t * pin,
	uint8_t pinn
	);
	
	//Initialize Sonar Object
	SONAR sonar_tmp;
	sonar_tmp.port = port;
	sonar_tmp.ddr = ddr;
	sonar_tmp.pin = pin;
	sonar_tmp.pinn = pinn;
	
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
