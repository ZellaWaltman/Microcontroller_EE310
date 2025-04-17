/*
 * The purpose of this program is demonstrate INT0 as an external interrupt. 
 * In this case when the signal changes state on RB0 (INT0) then D0 starts 
 * blinking for 4 seconds and then stops. The results can be simulated and
 * verified.  
 * 
 * Author: Zella Waltman
 */

#include <xc.h> // must have this
#include "ConfigureFile.inc"

//------------------------------------------
// Definitions, Variables, and Declarations:
//------------------------------------------

#define BlinkLED PORTDbits.RD0
#define WdtLED PORTDbits.RD1
#define PORTD_DIR TRISD
#define OUTPUT 0

unsigned int count = 0;

void Dummy_Stuck(void);

//------------
// Functions
//------------

void InitializePorts(void)
{
    //LATD= 0b00000000; // Initialize LATD
    PORTD= 0b00000000; // Initialize PORTD
    ANSELD= 0b00000000; // Digital I/0
    TRISD= 0b00000000; // sets PORTD as all outputs
}

//-----------------
// Main Code
//-----------------

void main(void) {
    
    InitializePorts(); // Setup Port D as an Output & Initializes to OFF
    
    // Watchdog Timer
    WDTCON0 = 0b00011011; // Watchdog Timer Prescale Rate, 1:65536 (Interval 2s nominal)
    
    __delay_ms(1000); // Initial Delay

    count = 0;
    BlinkLED = 0;
    WdtLED = 0;
    
    while(1)
    {
        BlinkLED = !BlinkLED; // Toggle LED
        __delay_ms(100); // Delay 100 ms
        CLRWDT(); // Clear Watchdog Timer
        count++; // increment count by 1
        
        if (count > 5)
        {
            WdtLED = 1; // Stay high indicating system is idle
            Dummy_Stuck(); // Dummy_Stuck loop function
        } 
    }
}

void Dummy_Stuck(void)
{
  //while(1); // Wait till watchdog timeout and then reset system
    
    SLEEP(); // Sleep function

}