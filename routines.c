/* routines.c
 * 
 * Module contains the logic of the
 * various operation modes of the robot
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

// Definitions
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

//Include standard headers
#include <stdint.h>			// For data types
#include <util/delay.h>		// For delays in victory dance

//Include Local Headers
#include "routines.h"		// Header for this file
#include "motor.h"			// For motor control
#include "sonar.h"			// For sonar sensing
#include "infrared.h"		// For infrared sensing
#include "qti.h"			// For border sensing
#include "indicator.h"		// For LED and buzzer control

// Constants
#define MOMENTUM_SWITCH_DIST 8	// Distance required to reverse momentum
#define MAX_TRACKING_MISSES 3	// Number of times the object isn't seen before
								// We give up and go back to searching
#define MAX_TRACKING_BOUNDS 2	// Number of times the bounds are seen before
								// We give know it's not a glitch.
#define SPOTTED_THRESHOLD 3		// Number of times something must be spotted before attacking
#define SPINOFF_COUNT 2048		// Number of counts to execute a spinoff
#define EVADE_COUNT 2048		// Number of counts to execute an evade maneuver
#define BACKUP_COUNT 2048		// Number of counts to back away from edge

/* Begin Global Functions here
 * ==========================
 *
 */

// Operate the robot in search mode
void routines_search(void){

	// Initialize object detection variable
	int8_t objDetected = 0;
	int8_t oldobjDetected = 0;

	// Copy travel direction
	int8_t direction = motor_dirTravelA;

	// Determine which direction to move
	if(motor_dirTurnA == -1){
		// If turning left, keep turning left
		motor_setSpeed(2*direction,3*direction);
	}else{
		// If turning right, keep turning right
		motor_setSpeed(3*direction,2*direction);
	}

	uint8_t spotted = 0;
	
	while (spotted<SPOTTED_THRESHOLD){
		// While no object is detected,
		// Keep searching and switch directions when necessary

		while(!qti_touchingBounds && spotted<SPOTTED_THRESHOLD){
			// While no bounds or objects are detected

			// Get sonar measurement
			oldobjDetected = objDetected;
			objDetected = sonar_getRegion();
			
			if(sonar_isNewDist(objDetected)){
				if((objDetected==oldobjDetected) && objDetected){
					// Check if the object is in the direction we're charging
					// If it's not, count a miss.
					spotted++;
				}else{
					spotted = 0;
				}
			}

		}
		
		if(!objDetected){
			if (motor_dirTravel != 0){
				// If we are actually moving in some direction
				if (qti_touchingBounds == motor_dirTravel){
					// If a boundary is detected in the direction of travel
					// Reverse direction of travel and turning
					motor_setSpeed(-motor_currentSpeed[1],-motor_currentSpeed[0]);

				}else if(qti_touchingBounds == -motor_dirTravel){
					// If a boundary is detected opposite the direction of travel
					// Then we are being pushed out of the ring

					// Perform routine in attempt to spin off
					//routines_spinOff(qti_touchingBounds);
				}
			}else{
				// If we are spinning
				// Then switch spin direction
				motor_setSpeed(-motor_currentSpeed[0],-motor_currentSpeed[1]);
			}
		}
	}

	//if(objDetected == -motor_dirTravel){
		// If the object is detected opposite the direction of travel
			//if (sonar_getDistance(0xFF)<MOMENTUM_SWITCH_DIST){
				// If the the opponent is to close to reverse direction
				// Make a move to evade
				//routines_evade(objDetected);

			//}else{
				// Switch direction and attack
				//routines_attack(objDetected);
			//}
	//}else{
		// The object is found in direction of travel, go get it.
		routines_attack(objDetected);

	//}
		
}

// Operate the robot in attack mode
void routines_attack(int8_t direction){

	// Charge the opponent
	motor_setSpeed(3*direction,3*direction);

	// Initialize the variable showing where the object was seen
	uint8_t missCounter = 0;
	uint8_t boundCounter = 0;
	
	while((missCounter<MAX_TRACKING_MISSES) && (boundCounter<MAX_TRACKING_BOUNDS)){
		// While we haven't lost sight more consecutive a number times
		
		// Look for consecutive misses
		if sonar_isNewDist(direction){
			// If there is a new distance measurement
			if(sonar_getRegion()!=direction){
				// Check if the object is in the direction we're charging
				// If it's not, count a miss.
				missCounter++;
			}else{
				// If it is,reset the miss counter.
				missCounter = 0;
			}
		}

		// Looking for consecutive touchingBounds
		if(qti_touchingBounds){
			// Increment the bound counter if we see a bound
			// This is meant to stray signals
			boundCounter++;
		}else{
			// Reset the bound counter if no bound is found
			boundCounter = 0;
		}
			
	}
	if(qti_touchingBounds){
		// If a bound was touched
		if(qti_touchingBounds==direction){
			// If a bound was touched on the pushing side
			// Opponent should be out of the ring
			//routines_victoryDance(-direction);

		}else if(qti_touchingBounds==-direction){
			// If a bound was touched on the back side
			// We're being pushed out, try to spin out.
			//routines_spinOff(qti_touchingBounds);

		}
	}
}

// Avoid being rear-ended if we don't have time to reverse momentum
void routines_evade(int8_t direction){
	// Determine how to execute the spin
	if(motor_dirTurn == -1){
		// If turning left, hold left motor back slightly
		// to make the turn sharper
		motor_setSpeed(direction,3*direction);
	}else{
		// If turning right, hold right motor back slightly
		// to make the turn sharper
		motor_setSpeed(3*direction,direction);
	}

	// Wait the correct amount of time.
	uint16_t spinCounter = 0;
	for(spinCounter=0; spinCounter<EVADE_COUNT; spinCounter++){
		// For the specified number of counts, complete the turn.

		//Keep polling sensors so we have current data when we're done
		sonar_getDistance(0);
	}
}

// Torero move to avoid being pushed out
void routines_spinOff(int8_t direction){

	// Determine how to execute the spin
	if(motor_dirTurn == -1){
		// If turning left, set left motor back slightly
		// and set right to max
		motor_setSpeed(-direction,3*direction);
	}else{
		// If turning right, set right motor back slightly
		// and set left to max
		motor_setSpeed(3*direction,-direction);
	}

	// Wait the correct amount of time.
	uint16_t spinCounter = 0;
	for(spinCounter=0; spinCounter<SPINOFF_COUNT; spinCounter++){
		// For the specified number of counts, complete the turn.

		//Keep polling sensors so we have current data when we're done
		sonar_getDistance(0);
	}
}

// Do a victory dance
void routines_victoryDance(int8_t direction){
	
	// Backup from the edge of the circle
	motor_setSpeed(3*direction, 3*direction);

	// Keep backing up until the count is done
	uint16_t i_backup;		// Initialize iterator
	for(i_backup=0; i_backup<BACKUP_COUNT; i_backup++){
		_delay_us(5);
	}

	// Start spinning around
	motor_setSpeed(1,-1);

	while(1){
		indicator_beep(); 			// Beep
		_delay_ms(200);				// Wait
		indicator_greenFlash(50);	// Flash
		_delay_ms(200);				// Wait
	}								// Repeat
}
