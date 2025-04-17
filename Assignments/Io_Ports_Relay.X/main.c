/*
 * ---------------------
 * Title: IO Ports Relay
 * ---------------------
 * Program Details:
 *      This program
 * 
 * Inputs: PORTB
 * PORTB Mapping:
 *  RB0 - Interrupt Button
 *  RB1 - Photoresistor 1 (First Input)
 *  RB2 - Photoresistor 2 (Second Input)
 *  RB5 - Confirm Code Entry button
 * 
 * 
 * 
 * Outputs: PORTC, PORTD (7 Segment0
 * PORTC Mapping:
 *   RC3 - System ON LED
 *   RC5 - Motor Output, correct code
 *   RC6 - Incorrect/Interrupt Buzzer
 * 
 * PORTD Mapping (7 Segment):
 *   RD0 - A
 *   RD1 - B
 *   RD2 - C
 *   RD3 - D
 *   RD4 - E
 *   RD5 - F
 *   RD6 - G
 *   RD7 - DP - Indicates second number is pressed
 * 
 * Setup: C- Simulator
 * Date: April 4th, 2025
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File, Functions Header File, and Initialization Header File 
 * Compiler: xc8, 3.0
 * Author: Zella Waltman
 * Versions:
 *      V1.0: Original
 * Useful links:  
 *      Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
 *      PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
 *      List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 
 */

//---------------------
// Initialization
//---------------------


//#include <stdio.h>
//#include <stdlib.h>
//#include <stdint.h> // For standard integer types like uint8_t

#include <xc.h> // must have this
#include "ConfigureFile.inc"
#include "Initialization.h"
#include "Functions.h"

int main(void) {
    
    initialization(); // Initialize everything, located in "Initialization.h"
    
    while(1)
    {
        UserInput();
        DetermineInput();
    }
}  