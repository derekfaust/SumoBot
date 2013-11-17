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

// Function to initialize PWM
void motor_init(void);

// Function to set motor speed
void motor_setSpeed(uint8_t motornum, int8_t speed);
/* motornum [0,1]:
 * right	0
 * left		1
 *
 * speed [-3,3]:
 * full reverse	-3
 * stopped		 0
 * full forward  3
 */

// Function to get motor speed
uint8_t motor_getSpeed(uint8_t motornum);
/* motor numbers and speeds are the same as
 * in the setSpeed function
 */

#endif
