/*
 * ------------------------------------------------
 * Keypad: Header File
 * --------------------------------------------------

 * This is a header file which contains functions for the Keypad.asm file
 * These functions include:
 *    _setupPortD: Sets up PORTD for Output
 *    _setupPortB: Sets up PORTB for Input
 *    _check_keypad: Checks which button is being pressed on the keypad
    
*/

#include <xc.h> // include processor files - each processor file is guarded.  
#include "ConfigureFile.inc"

// This is a guard condition so that contents of this file are not included
// more than once.  
//#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

//-------------------------------------
// Call Functions
//-------------------------------------

void InitializePorts(void){
    LATD= 0b00000000; // Initialize LATD
    PORTD= 0b00000000; // Initialize PORTD
    ANSELD= 0b00000000; // Digital I/0
    TRISD= 0b00001111; // Sets Rows as Outputs, Columns as Inputs
    
    LATB= 0b00000000; // Initialize LATB
    PORTB= 0b00000000; // Initialize PORTB
    ANSELB= 0b00000000; // Digital I/0
    TRISB= 0b00000000; // sets PORTD as all outputs
    }

void InitializeREG(void) {
    int Operation_REG = 0;
    int Display_Result_REG = 0;
    int X_Input_REG = 0;
    int Y_Input_REG = 0;
    int num1 = 0;
    int num2 = 0;
    int secondNum = 0;
}

int Addition(int X, int Y) {
    return X + Y;
}

int Subtraction(int X, int Y) {
    return X - Y;
}

int Multiplication(int X, int Y) {
    return X * Y;
}

int Division(int X, int Y) {
    return X / Y;
}
