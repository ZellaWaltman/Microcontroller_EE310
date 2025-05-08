  /*
 * ------------------------------------------------
 * IO_Ports_Relay: Initialization Header File
 * --------------------------------------------------

 * This is a header file which initializes all variables and contains the
 * Initialization functions for the main.asm file. These functions include:
 *    InitializePorts: Sets up PORTD and PORTB
 *    INTERRUPT_Initialize: Initializes the Interrupt Bits
 *    __interrupt: Defines Interrupt ISR - BUZZER GOING OFF
 *    
*/

//------------------------------
// Definitions
//------------------------------



//------------------------------
// Declaring Functions
//------------------------------
void initialization();
void InitializePorts();

//------------------------------
// Initialization Functions
//------------------------------

void initialization()
{
    InitializePorts();
}

void InitializePorts()
{
    // Initializing Port B
    PORTB = 0; // Initialize PORTB
    TRISB = 0b00000011; // sets RB0, RB1 as inputs
    LATB = 0; // Initialize LATB
    //ANSELB = 0;
    ANSELB = 0b00000000; // RB1 and RB2 are Analog I/O for the ADC. Rest are Digital I/O.
    
    // Initializing Port D
    PORTD = 0; // Initialize PORTD
    TRISD = 0; // PORTD is an Output
    LATD = 0; // Initialize LATD
    ANSELD = 0; // Digital I/O
    
}
