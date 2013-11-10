/* infrared.h
 * 
 * Header file defines all interfaces between
 * the infrared module and the main program
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

//Define functions if undefined
#ifndef INFRARED_H
#define INFRARED_H

//Include standard data types
#include <stdint.h>		

// Function to initialize all IR sensors.
void infrared_init();

// Function to get which region and object is detected in.
int8_t infrared_getRegion();

#endif
