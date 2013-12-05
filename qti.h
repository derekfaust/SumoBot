/* qti.h
 * 
 * Header file defines all interfaces between
 * the qti module and the main program
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

//Define functions if undefined
#ifndef QTI_H
#define QTI_H

// Include standard libraries
#include <stdint.h>			// For data types

//Define global variables
extern volatile int8_t qti_touchingBounds;
/* Variable indicating which end of the robot is touching
 * the out-of-bounds line.
 *
 * This variable is updated by a pin-change interrupt on any of the QTI sensors

 * Output Information
 * ------------------
 * Front	 1
 * Back		-1
 * Both		 1
 * Neither	 0
 */

// Function to initialize all QTI sensors.
void qti_init(void);

#endif
