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

// Function to get a map locating where
// around the periphery object are detected.
uint8_t infrared_activeMap();

// Function to get a map locating where
// around the periphery stay IR is detected
uint8_t infrared_passiveMap();

#endif
