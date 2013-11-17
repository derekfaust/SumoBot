/* routines.c
 * 
 * Module contains the logic of the
 * various operation modes of the robot
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

//Include standard headers
#include <stdint.h>			// For data types

//Include Local Headers
#include "routines.h"		// Header for this file
#include "motor.h"			// For motor control
#include "sonar.h"			// For sonar sensing
#include "infrared.h"		// For infrared sensing
#include "qti.h"			// For border sensing

// Constants
#define MOMENTUM_SWITCH_DIST 5	// Distance required to reverse momentum
#define MAX_TRACKING_MISSES 5	// Number of times the object isn't seen before
								// We give up and go back to searching

/* Begin Global Functions here
 * ==========================
 *
 */

// Operate the robot in search mode
int8_t routines_search(void){
	int8_t objDetected = 0;		// Initialize object detection variable
	while (!objDetected){
		// While no object is detected,
		// Keep searching and switch directions when necessary

		while(!qti_touchingBounds && !objDetected){
			// While no bounds or objects are detected

			// Get sonar measurement
			objDetected = sonar_getRegion();

			// Use IR sensors too
			
		}
		
		if (qti_touchingBounds == motor_dirTravel){
			// If a boundary is detected in the direction of travel
			// Reverse direction of travel and turning
			motor_setSpeed(-motor_currentSpeed[1],-motor_currentSpeed[0]);

		}else if(qti_touchingBounds == -motor_dirTravel){
			// If a boundary is detected opposite the direction of travel
			// Then we are being pushed out of the ring

			// Perform routine in attempt to spin off
		}
	}

	if(objDetected == -motor_dirTravel){
		// If the object is detected opposite the direction of travel
			if (sonar_getDistance(0xFF)<MOMENTUM_SWITCH_DIST){
				// If the the opponent is to close to reverse direction
				// Make a move to evade
				routines_evade(objDetected);

			}else{
				// Switch direction and attack
				routines_attack(objDetected);
			}
	}else{
		// The object is found in direction of travel, go get it.
		routines_attack(objDetected);

	}
		
}

// Operate the robot in attack mode
int8_t routines_attack(int8_t direction){

	// Charge the opponent
	motor_setSpeed(3*direction,3*direction);

	// Initialize the variable showing where the object was seen
	uint8_t missCounter = 0;
	uint8_t boundCounter = 0;
	
	while((missCounter<MAX_TRACKING_MISSES) && (boundCounter<MAX_TRACKING_BOUNDS)){
		// While we haven't lost sight of the opponent 5 or more times
		if(sonar_getRegion()!=direction){
			// Check if the object is in the direction we're charging
			// If it's not, count a miss.
			missCounter++;
		}
		if(qti_touchingBounds){
			// Increment the bound counter if we see a bound
			// This is meant to stray signals
			boundCounter++;
		}
			
	}
	if(qti_touchingBounds){
		// If a bound was touched
		if(qti_touchingBounds==direction){
			// If a bound was touched on the pushing side
			// Opponent should be out of the ring

		}else if(qti_touchingBounds==-direction){
			// If a bound was touched on the back side
			// We're being pushed out, try to spin out.

		}
	}
}

// Avoid being rear-ended
void routines_evade(int8_t direction){

}
