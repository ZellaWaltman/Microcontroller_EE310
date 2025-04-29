/*
 * ------------------------------------------------
 * ADC and LCD: Initialization Header File
 * --------------------------------------------------

 * This is a header file which initializes all variables and contains the
 * Initialization functions for the main.asm file. These functions include:
 *    InitializePorts: Sets up PORTD and PORTB
 *    INTERRUPT_Initialize: Initializes the Interrupt Bits
 *    __interrupt: Defines Interrupt ISR - BUZZER GOING OFF
 *    ADC_Init(): Initializes the Analog to Digital Converter
 *    InitializeLCD(): Initializes the 16x2 LCD Display
 *    INTERRUPT_Initialize: Initializes the Interrupt Bits
 *    __interrupt: Defines Interrupt ISR - Blinking LED
*/

//------------------------------
// Function Declarations
//------------------------------

void ADC_Init();
void InitializePorts();
void __interrupt(irq(IOC), base(0x4008)) ISR(void);
void INTERRUPT_Initialize (void);

//------------------------------
// Initialization Functions
//------------------------------

void InitializePorts()
{
    // Initializing RA0
    PORTAbits.RA0 = 0; // Initialize RA0
    TRISAbits.TRISA0 = 0; // RA0 is an Output
    LATAbits.LATA0 = 0; // Initialize RA0
    ANSELAbits.ANSELA0 = 0; // Digital I/O
    
    // Initializing RA1
    PORTAbits.RA1 = 0; // Initialize RA1
    TRISAbits.TRISA1 = 1; // RA1 is an input
    LATAbits.LATA1 = 0; // Initialize RA0
    ANSELAbits.ANSELA1 = 1; // RA1 is an Analog Value
    
    // Initializing Port B
    PORTB = 0; // Initialize PORTB
    TRISB = 0; // PORTB is an Output
    LATB = 0; // Initialize LATB
    ANSELB = 0; // Digital I/O
    
    // Initializing RC2
    PORTCbits.RC2 = 0; // Initialize RC2
    TRISCbits.TRISC2 = 1; // RC2 is an Input
    LATCbits.LATC2 = 0; // Initialize RC2
    ANSELCbits.ANSELC2 = 0; // Digital I/O
    
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
   
    ADPCHbits.PCH = 1; // Set RA1 as Analog channel

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


void __interrupt(irq(IOC), base(0x4008)) ISR(void) {
    if (IOCCFbits.IOCCF2) {
        // Handle the RC2 (IOCC2) interrupt event
        // Blinking LED sequence
        for (int count = 0; count < 20; count++){ // Repeat ten times
            PORTAbits.RA0 = 1;
            __delay_ms(500); // 500 ms delay
            PORTAbits.RA0 = 0;
            __delay_ms(500); // 500 ms delay
        }
        
        IOCCFbits.IOCCF2 = 0; // Clear the flag after handling
    }
}

void INTERRUPT_Initialize (void)
{   
    
    // Using Page 134 Example in the PIC18F47K42 Datasheet:
    INTCON0bits.IPEN = 1;  // Enable interrupt priority
    INTCON0bits.GIEH = 1;  // Enable high priority interrupts
    INTCON0bits.GIEL = 1;  // Enable low priority interrupts
    
    PIE0bits.IOCIE = 1;    // Enable Interrupt-on-Change interrupts
        
    IOCCN = 0x00;          // Disable negative edge triggers on PORTC

    IOCCPbits.IOCCP2 = 1;  // Enable positive edge interrupt for RC2
    
    IOCCFbits.IOCCF2 = 0; // Clear the interrupt flag for IOCCF
    
    // Changing IVTBASE:
    IVTBASEU = 0x00; // Set IVTBASEU to 0x00 
    IVTBASEH = 0x40; // Set IVTBASEH to  0x40, default is 0x0008
    IVTBASEL = 0x08; // Set IVTBASEL to 0x08
}
