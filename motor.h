/* motor.h
 * 
 * Header file defines all interfaces between
 * motor module and the main program
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

// Define if undefined
#ifndef MOTOR_H
#define MOTOR_H

// Include standard data types
#include <stdint.h>

// Initialize Global Variables
extern int8_t motor_dirTurn;			// Current direction of turn
extern int8_t motor_dirTravel;			// Current direction of travel
extern int8_t motor_dirTurnA;			// Current/last direction of turn (non-zero)
extern int8_t motor_dirTravelA;			// Current/last direction of travel (non-zero)
extern int8_t motor_currentSpeed[2];	// Current speed setting of motors

// Function to initialize PWM
void motor_init(void);

// Function to set motor speed
void motor_setSpeed(int8_t speed0, int8_t speed1);
/* speed(0,1) [-3,3]:
 * full reverse	-3
 * stopped		 0
 * full forward  3
 */

#endif
