 /*
 * ------------------------------------------------
 * IO_Ports_Relay: Functions Header File
 * --------------------------------------------------

 * This is a header file which contains functions for the main.asm file
 * These functions include:
 *    _setupPortD: Sets up PORTD for Output
 *    _setupPortB: Sets up PORTB for Input
 *    _check_keypad: Checks which button is being pressed on the keypad
 */

//------------------------------
// Declaring Functions
//------------------------------

//void UserInput();
//void DetermineInput();

//------------------------------
// Functions
//------------------------------

void sevenSeg(int index, int secondNumInput)
{
    if ( index < 0 || index > 9 )
    {
        index = 0;
    }
    
    PORTD = sevenSegValues[index]; // Output value on seven segment based on index number
    
    if (secondNumInput == 1) // if Second Number is being entered
    {
        PORTD |= 0b10000000; // Turn on little dot to indicate second number
    }
}

//----------------------------------------------------
// Analog to Digital Conversions for Photoresistors
//----------------------------------------------------

/*int readADC(unsigned char channel)
{
    ADPCH = channel; // Select analog channel (e.g., 0x09 for RB1)
    __delay_us(10); // Acquisition delay
    ADCON0bits.GO = 1; // Start conversion
    while (ADCON0bits.GO); // Wait for completion
    light = (ADRESH*256) | ADRESL; // Combine Upper & Lower
    PRESS = light * 1.2;
}

int PR1Pressed(void)
{
    int value = readADC(0b00001001); // Set RB1 as Analog channel
    ADCValue = (ADRESH*256) | ADRESL; // Get ADC Value
    if (ADCValue >= PRESS) // If PR1 is Pressed
    {
        return 1;
    }
    else
    {
        return 0; // ~3 V, if ADC is above this value = pressed (dark)
    }
}

int PR2Pressed(void)
{
    int value = readADC(0b00001010); // Set RB2 as Analog channel
    ADCValue = (ADRESH*256) | ADRESL; // Get ADC Value
    if (ADCValue >= PRESS) // If PR1 is Pressed
    {
        return 1;
    }
    else
    {
        return 0; // ~3 V, if ADC is above this value = pressed (dark)
    }
}*/

/*
int readADC(unsigned char channel)
{
    ADPCH = channel; // Select analog channel (e.g., 0x09 for RB1)
    __delay_us(10); // Acquisition delay
    ADCON0bits.GO = 1; // Start conversion
    while (ADCON0bits.GO); // Wait for completion
    return (ADRESH*256) | ADRESL; // Return 10-bit result
}

char PR1Pressed(void)
{
    int value = readADC(0b00001001); // Set RB1 as Analog channel
    return value >= PRESS; // ~3 V, if ADC is above this value = pressed (dark)
}

char PR2Pressed(void)
{
    int value = readADC(0b00001010); // Set RB2 as Analog channel
    return value >= PRESS; // ~3 V, if ADC is above this value = pressed (dark)
}
*/

//--------------------------------------
// Checking if Enter Button is Pressed
//---------------------------------------
char ConfirmPressed(void)
{
    // Check if button is pressed
    if (Confirm == 1)
    {
        __delay_ms(50); // Debounce delay

        // Check again to confirm it's still pressed
        if (Confirm == 1)
        {
            // Wait for release
            while (Confirm == 1);
            __delay_ms(50); // Debounce release

            return 1; // Button was pressed and released
        }
    }

    return 0; // Button was not pressed
}

//-----------------------------
// Read and Check User Inputs
//-----------------------------
void UserInput()
{
    Input1 = 0;
    Input2 = 0;
    FinalInput = 0;
    
        while (!ConfirmPressed()) // if Button 1 (Confirm Button) is NOT pressed
        {
            
            // First Digit Input
            if (PORTBbits.RB1 == 1) // Photoresistor 1 is pressed
            {
                
               

                __delay_ms(100); // Debounce Delay

                while (PORTBbits.RB1  == 1); // Wait until PR1 is released

                __delay_ms(100); // Debounce Delay
                
                Input1++;
                sevenSeg(Input1, 0); // Display 1st Input
                
            }

            // Second Digit Input
            if (PORTBbits.RB2 == 1) // Photoresistor 2 is triggered
            {


                __delay_ms(100); // Debounce Delay

                while (PORTBbits.RB2 == 1); // Wait until PR2 is released

                __delay_ms(100); // Debounce Delay
                
                Input2++;
                sevenSeg(Input2, 1); // Display 2nd Input, with DP on to indicate Second Digit
            }
        }
    
    // Once Confirm Button is pressed
    PORTD = 0; // Clear Display

    FinalInput = (Input1*10)+Input2; 

}

void DetermineInput()
{
    if (Code == FinalInput) // Turn Correct LED ON for Five Seconds if code is correct
    {
        PORTCbits.RC5 = 1;
        __delay_ms(5000);
        PORTCbits.RC5 = 0;
    }
    else // If Incorrect, turn on Buzzer for 3 seconds
    {
        PORTCbits.RC6 = 1;
        __delay_ms(3000);
        PORTCbits.RC6 = 0; 
    }
    
    FinalInput = 0;
    
}