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
//#include "../../../../../Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h"
//#include "C:\Program Files\Microchip\xc8\v2.40\pic\include\proc\pic18f46k42"

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4

// Defining Interrupt ISR 
void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void)
{
    // Check if interrupt flag for INT0 is set to 1
    if (PIR1bits.INT0IF == 1){
        // Blinking LED Sequence
        for (int count = 0; count < 10; count++){ // Repeat ten times
            PORTDbits.RD0 = 1;
            //__delay_ms(200); // 200 ms delay
            PORTDbits.RD0 = 0;
            //__delay_ms(200); // 200 ms delay
        }
    }
    
     PIR1bits.INT0IF = 0; // Clear the interrupt flag when done
     PORTDbits.RD0 = 0; // Make sure led on PORTDbits.RD0 is off

}

void INTERRUPT_Initialize (void)
{   
    // Using Page 134 Example in the PIC18F47K42 Datasheet:
    INTCON0bits.IPEN = 1; // Enable interrupt priority
    INTCON0bits.GIEH = 1; // Enable high priority interrupts
    INTCON0bits.GIEL = 1; // Enable low priority interrupts
    
    INTCON0bits.INT0EDG = 1; // Interrupt on rising edge of INT0 pin
    IPR1bits.INT0IP = 1; // Set the interrup high priority (IP) for INT0 - INT0IP
    PIE1bits.INT0IE = 1; // Enable the interrup (IE) for INT0
    
    PIR1bits.INT0IF = 0; // Clear the interrupt flag for INT0
    
    // Changing IVTBASE:
    IVTBASEU = 0x00; // Set IVTBASEU to 0x00 
    IVTBASEH = 0x40; // Set IVTBASEH to  0x40, default is 0x0008
    IVTBASEL = 0x08; // Set IVTBASEL to 0x08
}

void InitializePorts(void){
    LATB= 0b00000000; // Initialize LATB
    PORTB= 0b00000000; // Initialize PORTB
    ANSELB= 0b00000000; // Digital I/0
    TRISB= 0b00000001; // sets RB0 as an input
    
    LATD= 0b00000000; // Initialize LATD
    PORTD= 0b00000000; // Initialize PORTD
    ANSELD= 0b00000000; // Digital I/0
    TRISD= 0b00000000; // sets PORTD as all outputs
    }

void main(void) {

    // Initialization  
    InitializePorts(); // set port B and D as outputs 
    
    WPUB = 0xFF; // 0b11111111, enable the weak pull-ups for Port B
    
    INTERRUPT_Initialize(); // Initialize the interrupt_initialization by calling the proper function

    // Main Loop
    while(1){ // blink an LED connected to RD1 every 2 seconds - Will be interrupted on Rising Edge of RB0
        
        PORTDbits.RD1 = 1;
        //__delay_ms(2000); // 2000 ms (2 s) delay
        PORTDbits.RD1 = 0;
        //__delay_ms(2000); // 2000 ms (2 s) delay
        
    }
}
