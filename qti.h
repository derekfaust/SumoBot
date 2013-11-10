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

//Include standard data types
#include <stdint.h>		

// Function to initialize all QTI sensors.
void qti_init();

#endif
