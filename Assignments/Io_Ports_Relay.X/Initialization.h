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

#define Confirm PORTBbits.RB5
#define PR1 PORTBbits.RB1
#define PR2 PORTBbits.RB2

//------------------------------
// Initialization of Variables
//------------------------------

int Input1 = 0;
int Input2 = 0;
int Code = 0;
int FinalInput = 0;
int count = 0;

//---------------------------------
// Setting up Seven Segment Values
//---------------------------------
unsigned char sevenSegValues[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x00,0x79};

//------------------------------
// Declaring Functions
//------------------------------
void initialization();
void InitializePorts();
void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void);
void INTERRUPT_Initialize (void);

//------------------------------
// Initialization Functions
//------------------------------

void initialization()
{
    InitializePorts();
    INTERRUPT_Initialize();

    // Set the Secret Code
    Code = 13; // User will have to use PR1 once, and PR2 3 times
}

void InitializePorts()
{
    // Initializing Port B
    PORTB = 0; // Initialize PORTB
    TRISB = 0b00101111; // sets RB1, RB2, and RB3 as inputs
    LATB = 0; // Initialize LATB
    //ANSELB = 0;
    ANSELB = 0b00000000; // RB1 and RB2 are Analog I/O for the ADC. Rest are Digital I/O.
    WPUB = 0b00000001;   // Weak Pull-Ups for  RB0, RB1, and RB2.
    
    // Initializing Port C
    PORTC = 0; // Initialize PORTC
    TRISC = 0; // PORTC is an Output
    LATC = 0; // Initialize LATC
    ANSELC = 0; // Digital I/O
    
    // Initializing Port D
    PORTD = 0; // Initialize PORTD
    TRISD = 0; // PORTD is an Output
    LATD = 0; // Initialize LATD
    ANSELD = 0; // Digital I/O
    
}

// Defining Interrupt ISR - BUZZER GOING OFF
void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void)
{
    // Check if interrupt flag for INT0 is set to 1
    if (PIR1bits.INT0IF == 1){
        // Buzzer going off sequence
        for (int count = 0; count < 4; count++){ // Repeat ten times
            PORTCbits.RC6 = 1;
            __delay_ms(200); // 200 ms delay
            PORTCbits.RC6 = 0;
            __delay_ms(200); // 200 ms delay
        }
    }
    
     PIR1bits.INT0IF = 0; // Clear the interrupt flag when done
     PORTCbits.RC6 = 0; // Make sure Buzzer on RC6 is off

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
