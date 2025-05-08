/*
 * ------------------------------------------------
 * Simon Says Game: Initialization Header File
 * --------------------------------------------------
 *  This is a header file which initializes all variables and contains the
 * Initialization functions for the main.asm file. These functions include Port
 * Initialization (PORTA-D), and the setup for the Interrupt-on-Change
 * for PORTC (RC3-6).
*/

#include <stdbool.h>
#include "PWM.h"

//------------------------------
// Initialization of Variables
//------------------------------

// LED index remap: 0 = RA0, 1 = RA1, 2 = RA4 (instead of RA2), 3 = RA3; Pin RA2 is broken
const uint8_t led_pins[4] = {0, 1, 3, 4};

volatile uint8_t userInput[32];  // Stores user's button presses
volatile uint8_t userIndex = 0;  // Tracks number of inputs entered
volatile uint8_t lastButton = 0; // Stores last button pressed

volatile bool buttonPressed = false; // Set true by the ISR when a button is pressed
volatile bool gameStarted = false; // Game Start. Press any button to start.

//------------------------------
// Function Declarations
//------------------------------

void InitializePorts();
void __interrupt(irq(IOC), base(0x4008)) ISR(void);
void INTERRUPT_Initialize (void);

//------------------------------
// Initialization Functions
//------------------------------

void InitializePorts()
{
    
    // Initializing Port A
    PORTA = 0; // Initialize PORTA
    TRISA = 0; // PORTA (RA0-RA3) are Outputs
    LATA = 0; // Initialize LATA
    ANSELA = 0; // Digital I/O
    
    // Initializing Port B
    PORTB = 0; // Initialize PORTB
    TRISB = 0; // PORTB is an Output
    LATB = 0; // Initialize LATB
    ANSELB = 0; // Digital I/O
    
    // Initializing Port C
    PORTC = 0; // Initialize PORTC
    TRISC = 0x78; // PORTC (RC3-RC6) are Inputs
    LATC = 0; // Initialize LATC
    ANSELC = 0; // Digital I/O
    //WPUC = 0x3C;  // Enabling Weak Pull-ups for PORTC
    
    // Initializing Port D
    PORTD = 0; // Initialize PORTD
    TRISD = 0; // PORTD is an Output
    LATD = 0; // Initialize LATD
    ANSELD = 0; // Digital I/O
    
}

//---------------------------------
// Interrupt for buttons (RC3-RC6)
//---------------------------------

void __interrupt(irq(IOC), base(0x4008)) ISR(void) {
    if (PIR0bits.IOCIF) {  // Check for interrupt-on-change on PORTC
        for (uint8_t i = 3; i <= 6; i++) {  // Check RC3 to RC6
            if (IOCCF & (1 << i)) {
                uint8_t buttonIndex = i - 3;  // Map RC3 -> RC6 (0 -> 3)
                
                if (!gameStarted) {
                    gameStarted = true;
                } else {
                    // Play tone on button press
                    playButtonSound(buttonIndex);
                    
                    // Record button input during game
                    userInput[userIndex++] = buttonIndex;
                    lastButton = buttonIndex;
                    buttonPressed = true;
                }

                // Debounce delay
                __delay_ms(20);

                // Wait for button release
                while (PORTC & (1 << i));

                // Flash LED and play sound
                LED_PORT = (1 << led_pins[buttonIndex]);
                __delay_ms(300);
                LED_PORT = 0;

                IOCCF &= ~(1 << i);  // Clear that pin's interrupt flag
                break;  // Only handle one press per ISR
            }
        }

        PIR0bits.IOCIF = 0;  // Clear overall interrupt-on-change flag
        
    }
}

void INTERRUPT_Initialize (void)
{   

    INTCON0bits.GIE = 1;
    // Using Page 134 Example in the PIC18F47K42 Datasheet:
    INTCON0bits.IPEN = 1;  // Enable interrupt priority
    INTCON0bits.GIEH = 1;  // Enable high priority interrupts
    INTCON0bits.GIEL = 1;  // Enable low priority interrupts
    
    PIE0bits.IOCIE = 1;    // Enable Interrupt-on-Change interrupts
    PIR0bits.IOCIF = 0;   
    
    IOCCN = 0x00;          // Disable negative edge triggers on PORTC

    IOCCP = 0x78;  // Enable positive edge interrupt for RC3-RC6
    
    IOCCF = 0; // Clear the interrupt flag for IOCCF
    
    // Changing IVTBASE:
    IVTBASEU = 0x00; // Set IVTBASEU to 0x00 
    IVTBASEH = 0x40; // Set IVTBASEH to  0x40, default is 0x0008
    IVTBASEL = 0x08; // Set IVTBASEL to 0x08
}