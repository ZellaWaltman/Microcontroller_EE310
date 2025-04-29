/*
 * File:   main.c
 * Author: Zella Waltman
 * Purpose: The purpose of this program is demonstrate How the ADC operates.
 * A port will be used to represent the input voltage in binary.
 *
 * Created on April 22, 2025, 11:47 AM
 */

#include <xc.h>
#include "ConfigureFile.inc"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//------------------------------
// Definitions
//------------------------------

#define Vref 3.3 // voltage reference 

//------------------------------
// Initialization of Variables
//------------------------------

// ADC Variables
int digital; // holds the digital value 
float voltage; // hold the analog value (volt))
char data[10];

//------------------------------
// Function Declarations
//------------------------------

void ADC_Init();
void InitializePorts();

//-------------------------
// Main Function
//-------------------------

/*This code block configures the ADC for polling, VDD and VSS references, ADCRC
oscillator and AN0 input. Conversion start & polling for completion are included.*/

void main(void) {
    ADC_Init(); //ADC Initialization
    InitializePorts(); // Initialize Ports B and D for output
    
    while (1) {
        
        ADCON0bits.GO = 1; // Set ADCON0 Go to start conversion
        
        while (ADCON0bits.GO); // Wait for conversion to be done
        
        digital = (ADRESH*256) | (ADRESL); // Combines 8-bit LSB and 2-bit MSB
        
        // define voltage = Vref/4096 (note that voltage is float type)
        voltage = digital*((float)Vref/(float)4096);
       
        // Write code to translate the values from ADRESH:ADRESL register 
        // pair to IO Port. In this case we can connect ADRESL to Port D

        PORTD = ADRESL;
        PORTB = ADRESH;
        
        __delay_ms(2000);
        
        // This is used to convert integer value to ASCII string
        sprintf(data,"%.2f",voltage);
        strcat(data," V");	// Concatenate result and unit to print
    }
}

//------------------------------
// Initialization Functions
//------------------------------

void InitializePorts()
{
    // Initializing Port B
    PORTB = 0; // Initialize PORTB
    TRISB = 0; // PORTB is an Output
    LATB = 0; // Initialize LATB
    ANSELB = 0; // Digital I/O
    
    // Initializing Port D
    PORTD = 0; // Initialize PORTD
    TRISD = 0; // PORTD is an Output
    LATD = 0; // Initialize LATD
    ANSELD = 0; // Digital I/O
    
}

void ADC_Init(void)
{
    //Setup ADC
    ADCON0bits.ON = 1; // ADC is Enabled 
    ADCON0bits.FM = 1; // ADCON0 set to right-justified
    ADCON0bits.CS = 1; // Clock Supplied from ADCRC dedicated oscillator
    
    TRISAbits.TRISA0 = 1; // RA0 is an input
    ANSELAbits.ANSELA0 = 1; // RA0 is an Analog Value
    ADPCHbits.PCH = 0; // Set RA0 as Analog channel

    ADCLKbits.CS = 0; // Set ADC CLOCK Selection register to zero
    
    // Clear ADC Result registers
    ADRESL = 0;
    ADRESH = 0;
    
    // Set precharge select to 0 in register ADPERL & ADPERH
    ADPREL = 0;
    ADPREH = 0;
    
    // Set acquisition LOW and HIGH byte to zero 
    ADACQL = 0;
    ADACQH = 0;
    
    ADCON0bits.ON = 1; // Turn ADC ON
}