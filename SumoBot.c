/* SumoBot.c
 * 
 * Main program to operate the robot during competition
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

// Include general I/O functionality
#include <avr/io.h>

// Include custom modules that need to be initialized
#include "routines.h"
#include "indicator.h"
#include "infrared.h"
#include "motor.h"
#include "qti.h"
#include "sonar.h"

// Main code loop that never exits
int main(void){
	
	// Initialize modules
	indicator_init();
	infrared_init();
	motor_init();
	qti_init();
	sonar_init();

    while(1){
		// Keep performing the search routine, which leads
		// into other necessary routines.
        routines_search();
    }
}
