  /*
 * ------------------------------------------------
 * IO_Ports_Relay: Initialization Header File
 * --------------------------------------------------

 * This is a header file which contains the Initialization functions for the main.asm file
 * These functions include:
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

#define VREF 5.0 // voltage reference 
#define PRESS_THRESHOLD_V 2.0 // Threshold for a press
#define PRESS ((int)((PRESS_THRESHOLD_V / VREF) * 1023))

//------------------------------
// Initialization of Variables
//------------------------------

int Input1 = 0;
int Input2 = 0;
int Code = 0;
int FinalInput = 0;
int count = 0;
int light = 0;
int ADCValue = 0;

/*
// ADC Variables
int digital; // holds the digital value 
float voltage; // hold the analog value (volt))
char data[10];
*/

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
void ADC_Initialize(void);

//------------------------------
// Initialization Functions
//------------------------------

void initialization()
{
    InitializePorts();
    INTERRUPT_Initialize();
    ADC_Initialize();
    
    // Set the Secret Code
    Code = 13; // User will have to use PR1 once, and PR2 3 times
    
}

void InitializePorts()
{
    // Initializing Port A
    PORTA = 0; // Initialize PORTD
    TRISA = 0b00000001; // RA0 and RA1 are inputs
    LATD = 0; // Initialize LATD
    ANSELD = 0; // Digital I/O
    WPUA = 0b00000001;   // For RA0 (Confirm)
    
    // Initializing Port B
    PORTB = 0; // Initialize PORTB
    TRISB = 0b00101111; // sets RB1, RB2, and RB3 as inputs
    LATB = 0; // Initialize LATB
    //ANSELB = 0;
    ANSELB = 0b00000110; // RB1 and RB2 are Analog I/O for the ADC. Rest are Digital I/O.
    WPUB = 0b00000001;   // Weak Pull-Ups for  RB0, RB1, and RB2.
    
    // Initializing Port C
    PORTC = 0; // Initialize PORTD
    TRISC = 0; // PORTC is an Output
    LATC = 0; // Initialize LATD
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
        for (int count = 0; count < 10; count++){ // Repeat ten times
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

void ADC_Initialize(void)
{
    //Setup ADC
    ADCON0bits.ON = 1; // ADC is Enabled 
    ADCON0bits.FM = 1; // ADCON0 set to right-justified
    ADCON0bits.CS = 1; // Clock Supplied from ADCRC dedicated oscillator

    ADPCHbits.PCH = 0b00001001; // Set RB1 as Analog channel, will be switched in readADC function
    
    ADCLKbits.CS = 0x0; // Set ADC CLOCK Selection register to zero
    
    // Clear ADC Result registers
    ADRESL = 0x0;
    ADRESH = 0x0;
    
    // Set precharge select to 0 in register ADPERL & ADPERH
    ADPREL = 0;
    ADPREH = 0;
    
    // Set acquisition low and high byte to zero 
    ADACQL = 0;
    ADACQL = 0;
}
