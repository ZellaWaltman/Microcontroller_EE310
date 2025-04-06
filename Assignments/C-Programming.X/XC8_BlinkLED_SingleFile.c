/*
 * ---------------------
 * Title: Simple LED Blinking 
 * ---------------------
 * Program Details:
 *  The purpose of this program is to simply blink an LED 
 * Inputs: myDelay 
 * 
 * Outputs: RD0 (output)
 * Setup: C- Simulator
 * Date: Feb 24, 2023
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File 
 * Compiler: xc8, 2.4
 * Author: Farid Farahmand
 * Versions:
 *      V1.0: Original
 * Useful links:  
 *      Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
 *      PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
 *      List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 
 */

#include <xc.h> // must have this
#include "ConfigureFile.inc"
//#include "../../../../../Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h"
//#include "C:/Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h"
//#include "C:\Program Files\Microchip\xc8\v2.40\pic\include\proc\pic18f46k42"

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4


void main (void){
TRISB= 0b00000000;//sets PORTB as all outputs
PORTB= 0b00000000;//turns off PORTB outputs so that the LED is initially off

    while(1){
        PORTBbits.RB1=0;//turns on LED
        __delay_ms(500);//delays for 1 second
        //PORTD=0b00000011;//turns off LED
        PORTBbits.RB1=1;//turns on LED    
        __delay_ms(500);//delays for 1 second
        }
}

/*
unsigned int a;
unsigned int b;
unsigned int c;
void main (void) {
    a = 0x2;
    b = 0x3;
    c = a + b;
}
*/

/*
void main (void){
TRISD= 0b00000000;//sets PORTB as all outputs
PORTD= 0b00000000;//turns off PORTB outputs so that the LED is initially off

unsigned char z;

for (z=0; z<20; z++)
        PORTD = z;
while(1);
}
*/

/*
void MSDelay(unsigned int,unsigned int);
unsigned char outputValue;

#define SevenSeg PORTB
#define mybit PORTBbits.RB7

void main (void) {
    
    LATB= 0b00000000;
    PORTB= 0b00000000;//turns off PORTB outputs so that the LED is initially off
    ANSELB= 0b00000000;
    TRISB= 0b00000000;//sets PORTB as all outputs
   
    while(1)
    {
        outputValue = 0x55;
        SevenSeg = outputValue;
        MSDelay(25,165);
        SevenSeg++;
        mybit = 1;
        MSDelay(25,165);        
    }
}

void MSDelay(unsigned int itime1, unsigned int itime2)
{
    unsigned int i; unsigned int j;
    for (i=0; i<itime1; i++)
        for (j=0; j<itime2; j++);
}
*/

/*
unsigned char seed;
void main (void)
{
    PORTB = 0; LATB = 0; ANSELB = 0; TRISB = 0;
    
    PORTD = 0; LATD = 0; ANSELD = 0;
    TRISDbits.TRISD0 = 1; // bit 0 is an input
    
    seed = 1;
    
    while(1)
    {
        while (PORTDbits.RD0==0)
        {
            seed++;
            if (seed == 7)
                seed = 1;
            PORTB = seed;
        }
    }
}
*/

/*
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void main (void)
{
    int value1; int value2;
    for (int i=0; i<10; i++)
    {
        value1 = rand();
        value2 = 10*(float)value1/32767; //return integer
        LATB = value2;
        printf("Today's lucky number is %d\n", value2);
    }
}
*/