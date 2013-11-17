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

//Define variables
static volatile int8_t qti_touchingBounds;
/* Location
 * Front	1
 * Back		-1
 */

// Function to initialize all QTI sensors.
void qti_init(void);

#endif
