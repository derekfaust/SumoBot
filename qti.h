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
static volatile uint8_t qti_touchingBounds;
/* Bit	Location
 * 0	Front
 * 1	Back
 */

// Function to initialize all QTI sensors.
void qti_init(void);

#endif
