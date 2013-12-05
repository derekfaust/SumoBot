/* routines.h
 * 
 * Header file defines all interfaces between
 * the routines module and the main program
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

// Define if undefined
#ifndef ROUTINES_H
#define ROUTINES_H

// Include standard data types
#include <stdint.h>

// Operate the robot in search mode
void routines_search(void);

// Operate the robot in attack mode
void routines_attack(int8_t direction);

// Torero move to avoid being pushed out
void routines_spin(int8_t direction);

// Do a victory dance
void routines_victoryBack(int8_t direction);

#endif
