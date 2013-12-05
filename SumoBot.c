/* SumoBot.c
 * 
 * Main program to operate the robot during competition
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

// Include standard libraries
#include <avr/io.h>		// General I/O functionality

// Include custom modules that need to be initialized
#include "routines.h"	// Movements and associated logic
#include "indicator.h"	// Triggering indicators
#include "motor.h"		// Controlling motors
#include "qti.h"		// Detecting boundaries
#include "sonar.h"		// Detecting objects

// Main code loop that never exits
int main(void){
	
	// Initialize modules
	indicator_init();
	motor_init();
	qti_init();
	sonar_init();

    while(1){
		// Keep performing the search routine,
		// which leads into other necessary routines.
        routines_search();
    }
}
