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
			motor_setSpeed(-motor_currentSpeed[1],motor_currentSpeed[0]);

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
}

// Avoid being rear-ended
void routines_evade(int8_t direction){

}
