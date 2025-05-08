/*
 * ---------------------
 * Title: ADC and LCD
 * ---------------------
 * Program Details:
 *      This program is designed to use the ADC Converter in a PIC 18K47F42
 * microcontroller to determine the voltage output of a 10 k potentiometer
 * voltage divider and display this value on a 16x2 LCD display.
 * 
 * Inputs: ANA1 (RA1 - The ADC Input), PORTC (RC2 - IOC Interrupt button)
 * 
 * Outputs: PORTB, PORTD, RA0 (Interrupt LED)
 * 
 * PORTB Mapping:
 *   RB0 - LCD Register Select Pin
 *   RB1 - LCD Enable Pin
 * 
 * PORTD Mapping (LCD Display Data Pins D1-D7):
 *   RD0 - D0
 *   RD1 - D1
 *   RD2 - D2
 *   RD3 - D3
 *   RD4 - D4
 *   RD5 - D5
 *   RD6 - D6
 *   RD7 - D7
 * 
 * Setup: C-Simulator
 * Date: April 27th, 2025
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

#include <xc.h> // must have this
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "ConfigureFile.inc"
#include "Initialization.h"
#include "Functions.h"

//------------------------------
// Definitions
//------------------------------

#define Vref 5 // voltage reference 

//------------------------------
// Initialization of Variables
//------------------------------

// ADC Variables
int digital; // holds the digital value 
float voltage; // hold the analog value (volt))
char data[10];

//-------------------------
// Main Function
//-------------------------

void main(void) {
    INTERRUPT_Initialize(); // Interrupt Initialize
    ADC_Init(); // ADC Initialization
    InitializeLCD();
    InitializePorts(); // Initialize Ports B and D for output
    
    // Write First Line of LCD (Voltage):
    LCD_Position(1,0,"Input Voltage:  "); 
    
    // Write First Line 3 LCD (LUX):
    //LCD_Position(1,0,"Input Light:    "); 
    
    while (1) {
        
        ADCON0bits.GO = 1; // Set ADCON0 Go to start conversion
        
        while (ADCON0bits.GO); // Wait for conversion to be done
        
        digital = (ADRESH*256) | (ADRESL); // Combines 8-bit LSB and 2-bit MSB
        
        /*
         // VOLTAGE CONVERSION
        //------------------------
        
        // define voltage = Vref/4096 (note that voltage is float type)
        voltage = digital*((float)Vref/(float)4096);
       
        // This is used to convert integer value to ASCII string
        sprintf(data,"%.2f",voltage);
        strcat(data," V");	// Concatenate result and unit to print
        
        LCD_Position(2,0,data); // Print Voltage on 2nd line of LCD
        */
        
        // LUX CONVERSION
        //------------------------
        
        /*
        float lux = digital/10;
       
        __delay_ms(200);
        
        // This is used to convert integer value to ASCII string
        sprintf(data,"%.2f",lux);
        strcat(data," LUX");	// Concatenate result and unit to print
        
        LCD_Position(2,0,data); // Print LUX on 2nd line of LCD*/
    }
}