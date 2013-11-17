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

// Function to retrieve distance value from one sonar
// or the closest from all of them
uint8_t sonar_getDistance(uint8_t sonarnum);

// Function to get which region and object is detected in.
int8_t sonar_getRegion();
/* region 	[-1,1] or 127:
 * Nothing	0
 * Front	1
 * Back		-1
 * Both		0
 */

#endif
