/*
 * ---------------------
 * Title: Calculator with Keypad
 * ---------------------
 * Program Details:
 *      This program acts as a simple Calculator. This program will receive an input from a 4x4 Matrix keypad
 *  and perform either Addition, Subtraction, Multiplication, or Division, depending on which operation is selected.
 *  The calculated value will then be output as a 8-bit binary number on a series of LEDs.
 * 
 * Inputs: PORTD
 * PORTD Mapping (4x4 Keypad):
 *  RD0 - C1
 *  RD1 - C2
 *  RD2 - C3
 *  RD3 - C4
 *  RD4 - R1
 *  RD5 - R2
 *  RD6 - R3
 *  RD7 - R4
 * 
 * Outputs: PORTB (LEDs)
 *  Bit 0 - RB0
 *  Bit 1 - RB1
 *  Bit 2 - RB2
 *  Bit 3 - RB3
 *  Bit 4 - RB4
 *  Bit 5 - RB5
 *  Bit 6 - RB6
 *  Bit 7 - RB7
 * 
 * Setup: C- Simulator
 * Date: April 4th, 2025
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File 
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

int X_Input_REG;
int Operation_REG;
int Y_INPUT_REG;
int Display_Result_REG;

#include <xc.h> // must have this
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // For standard integer types like uint8_t
#include "ConfigureFile.inc"
#include "HeaderFile.h"

//---------------------
// Definitions
//---------------------
#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4

//-----------------------
// Setting Up the Keypad
//-----------------------
int keypadMap[4][4] = { // Keypad is a 4x4 Matrix
    {1, 2, 3, 11}, // 11 represents A on the Keypad
    {4, 5, 6, 12}, // 12 represents B on the Keypad
    {7, 8, 9, 13}, // 13 represents C on the Keypad
    {15, 0, 16, 14} // 15, 16, and 14 represent *, #, and D on the Keypad, respectively
};

//----------------------------------
// Checking Which Keys are Pressed:
//----------------------------------
int keypad_getKey(void) {
    
    // CHECKING THE ROW
    //-------------------
    
    // unsigned integer of length 8 bits "row" is incremented 4 times,
    // one for each row of the keypad.
    for (uint8_t row = 0; row < 4; row++) {
        
        // LATD = 1; shift 1 to the left "row" + 4 bits. 
        // Ex: If row = 0, LAT will be 00010000. This is
        // ANDed with 1111000 to only get the upper nibble.
        LATD = (1 << (row + 4)) & 0xF0;

        __delay_ms(50); // Debounce delay
        
        // CHECKING THE COLUMN
        //-------------------
        
        // unsigned integer of length 8 bits "col" is incremented 4 times,
        // one for each column of the keypad.
        for (uint8_t col = 0; col < 4; col++) {
            // Shift 1 to the left "column" bits. Ex: 0001 -> 0010 -> 0100
            if (PORTD & (1 << (col))) {  // Checking RD0-RD3
                while ((PORTD & (1 << (col)))); // If Column Returns HIGH
                __delay_ms(50); // Debounce
                return keypadMap[row][col]; // Returning Keypad Value in Matrix
            }
        }
    }
    return -1; // No key pressed
}

int main(void) {
    InitializePorts(); // Initialize PortB, PortD, located in "HeaderFile.h"
    
    InitializeREG(); // Initialize REG, located in "HeaderFile.h"
    
    // Declaring Variables:
    int X_Input_REG, Y_Input_REG, num1, num2, Operation_REG, Display_Result_REG = 0;
    int secondNum = 0;
        
    while(1)
    {   
   
        int key = keypad_getKey();
        
        if (key != -1) { // If Key is Pressed
                    
            // If first digit is pressed 
            if (key >= 0 && key <= 9) { // is it a digit?
                if (!secondNum) { // If first number is being entered, not second
                    LATB = 0x80; // 1st light ON
                    if (num1 == 0) { // If this is first digit
                        num1 = key;
                    }
                    else { //If this is second digit
                        num1 = num1*10+key; // accumulate the input number, move 1st digit to tens place
                    }
                }
                else if (secondNum) { // If this is the second number
                    LATB = 0x40; // 2nd light ON
                    if (num2 == 0) { // If this is first digit
                        num2 = key;
                    }
                    else {
                        num2 = num2*10+key; // accumulate the input number 
                    }
                }
            }
            // IF AN OPERATION IS PRESSED:
            else if (key >= 10 && key <= 14) { // If A, B, C, or D is pressed, num1 is fully input
                X_Input_REG = num1;
                Operation_REG = key;
                secondNum = 1;
                
            }
            
            // Reset if * is pressed
            else if (key == 15) { // If * is pressed
                num1 = num2 = X_Input_REG = Y_Input_REG = Display_Result_REG = Operation_REG = secondNum = 0;
                LATB = 0;
            }
            
            // = if # is pressed
            else if (key == 16) { // If # is pressed
            Y_Input_REG = num2;
            
            switch (Operation_REG){
                case 11: // Addition
                    Display_Result_REG = Addition(X_Input_REG,Y_Input_REG);
                    break;
                case 12: // Subtraction
                    Display_Result_REG = Subtraction(X_Input_REG,Y_Input_REG);
                    break;
                case 13: // Multiplication
                    Display_Result_REG = Multiplication(X_Input_REG,Y_Input_REG);
                    break;
                case 14: // Division
                    Display_Result_REG = Division(X_Input_REG,Y_Input_REG);
                    break;
                default: Display_Result_REG = 0;
            }
            
            scanf("%d", &Display_Result_REG); // Make sure the result is an INT that we are entering into LATB
            LATB = Display_Result_REG;
            
            num1 = num2 = X_Input_REG = Y_Input_REG = Display_Result_REG = Operation_REG = secondNum = 0; // Reset Variables for next input
            
            }
        }
    }
}