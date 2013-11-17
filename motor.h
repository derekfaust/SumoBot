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

// Global Variables
extern int8_t motor_dirTurn;
extern int8_t motor_dirTravel;

// Function to initialize PWM
void motor_init(void);

// Function to set motor speed
void motor_setSpeed(int8_t speed0, int8_t speed1);
/* speed(0,1) [-3,3]:
 * full reverse	-3
 * stopped		 0
 * full forward  3
 */

// Function to get motor speed
uint8_t motor_getSpeed(uint8_t motornum);
/* Motor numbers and speeds are the same as
 * in the setSpeed function
 */

// Function to get turning direction
int8_t motor_getDirection(void);
/* Staight	0
 * Left		-1
 * Right	1
 */

#endif
