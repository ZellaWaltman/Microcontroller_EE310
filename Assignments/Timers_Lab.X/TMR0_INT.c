/*
 * The purpose of this program is demonstrate INT01 as an external interrupt.
 * In this case when the signal changes state on RB0(INT01) then D0 starts
 * blinking for 4 seconds and then stops. The results can be simulated and
 * verified.
 *
 * Author: Farid Farahmand
 */

#include <xc.h> // must have this
#include "ConfigureFile.inc"
// You may need this - change the path
//#include "../../../../../Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h"
//#include "C:\Program Files\Microchip\xc8\v2.40\pic\include\proc\pic18f46k42"

uint8_t checkTimerValue;
#define myLED  PORTDbits.RD1

void (*TMR0_InterruptHandler)(void);

void TMR0_Initialize(void)
{
    // Set TMR0 to the options selected in the User Interface

    // T0CS FOSC/4; T0CKPS 1:256; T0ASYNC synchronised; 
    T0CON1 = 0x48;

    // TMR0H ;  The total division will be (FOSC/4)/(4+1)x256)))
    TMR0H = 38; 

    // TMR0L 0; 
    TMR0L = 0x00;

    // Clear Interrupt flag before enabling the interrupt
    PIR3bits.TMR0IF = 0;

    // Enabling TMR0 interrupt.
    PIE3bits.TMR0IE = 1;

    // T0OUTPS 1:1; T0EN enabled; T016BIT 8-bit; 
    T0CON0 = 0x80;
}

void TMR0_StartTimer(void)
{
    // Start the Timer by writing to TMR0ON bit
    T0CON0bits.T0EN = 1;
}

void TMR0_StopTimer(void)
{
    // Stop the Timer by writing to TMR0ON bit
    T0CON0bits.T0EN = 0;
}

uint8_t TMR0_ReadTimer(void)
{
    uint8_t readVal;

    // read Timer0, low register only
    readVal = TMR0L;

    return readVal;
}

void TMR0_WriteTimer(uint8_t timerVal)
{
    // Write to Timer0 registers, low register only
    TMR0L = timerVal;
 }

void TMR0_Reload(uint8_t periodVal)
{
   // Write to Timer0 registers, high register only
   TMR0H = periodVal;
}

void __interrupt(irq(IRQ_TMR0), base(0x4008)) TMR0_ISR(void)
{
    T0CON0=0x00; // stop the timer & do what you have to do
    TMR0L=0; // reset the timer
    PIR3bits.TMR0IF = 0; // Clear the interrupt flag
    myLED ^= 1; // ISR code goes here
    T0CON0=0x80; //start the timer
    
}
void __interrupt(irq(default), base(0x4008)) DEFAULT_ISR(void)
{
// Unhandled interrupts go here
}
void INTERRUPT_Initialize (void)
{
INTCON0bits.GIEH = 1; // Enable high priority interrupts
INTCON0bits.GIEL = 1; // Enable low priority interrupts
INTCON0bits.IPEN = 1; // Enable interrupt priority

PIE3bits.TMR0IE = 1; // Enable TMR0 interrupt
PIE4bits.TMR1IE = 1; // Enable TMR1 interrupt

IPR3bits.TMR0IP = 0; // Make TMR0 interrupt low priority
// Change IVTBASE if required
IVTBASEU = 0x00; // Optional
IVTBASEH = 0x40; // Default is 0x0008
IVTBASEL = 0x08;
}

/**
  End of File
*/

void main (void) {
    ANSELD = 0b00000000;    
    TRISD= 0b00000000;//sets PORTB as all outputs 
    PORTD= 0b00000000;//turns off PORTB outputs so that the LED is initially off
    INTERRUPT_Initialize();
    TMR0_Initialize();
    TMR0_StartTimer();

    while (1) {
        PORTDbits.RD0=0;//turns on LED
        __delay_ms(1);//delays for 1 second
            //PORTD=0b00000011;//turns off LED
        PORTDbits.RD0=1;//turns on LED    
        __delay_ms(1);//delays for 1 second
    }
    
    
}
