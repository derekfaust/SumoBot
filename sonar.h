/* sonar.h
 * 
 * Header file defines all interfaces between
 * sonar module and the main program
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

//Define functions if undefined
#ifndef SONAR_H
#define SONAR_H

//Include standard data types
#include <stdint.h>		

// Function to initialize all sonar.
void sonar_init();

// Function to retrieve distance value from one sonar or
// an average from multiple that detect an object.
// Function will return zero if nothing is detected.
uint16_t sonar_getDistance(uint8_t sonarnum);

// Function to get which region and object is detected in.
int8_t sonar_getRegion();
/* region [-2,2]:
 * Far left	-2
 * Center	 0
 * Far Right 2
 */

#endif
