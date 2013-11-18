/* indicator.h
 * 
 * Header file defines all interfaces between
 * the indicator module and the main program
 *
 * MAE3780 Mechatronics Final Project 2013
 * Cornell University
 * Derek Faust, Nicole Panega, Abdullah Sayeem
 */

// Define functions if undefined
#ifndef INDICATOR_H
#define INDICATOR_H

// Include standard data types
#include <stdint.h>		

// Function to initialize indicator pins
void indicator_init(void);

// Function to make the buzzer beep
void indicator_beep(void);

// Function to flash the green LED
void indicator_greenFlash(uint8_t time);
/* Time is in units of 4ms
 */

// Function to flash the red LED
void indicator_redFlash(uint8_t time);
/* Time is in units of 4ms
 */

// Function to set the green LED
void indicator_greenSet(uint8_t state);
/* state	State
 * --------------
 * 0		Off
 * 1		On
 */

// Function to set the red LED
void indicator_redSet(uint8_t state);
/* state	State
 * --------------
 * 0		Off
 * 1		On
 */


#endif
