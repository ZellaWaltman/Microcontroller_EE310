 /*
 * ------------------------------------------------
 * IO_Ports_Relay: Functions Header File
 * --------------------------------------------------

 * This is a header file which contains functions for the main.asm file
 * These functions include:
 *    sevenSeg: Sets up Seven Segment increment and call
 *    ConfirmPressed: Checks if Enter Code Button has been pressed
 *    UserInput: Reads and Checks user inputs and increments input numbers, gets final code input
 *    DetermineInput: Checks if input is correct or incorrect, and behaves accordingly
 */

//------------------------------
// Declaring Functions
//------------------------------

//void UserInput();
//void DetermineInput();

//------------------------------
// Functions
//------------------------------

void sevenSeg(int index, int secondNumInput) // Seven Segment Display
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
                __delay_ms(200); // Debounce Delay

                while (PORTBbits.RB1  == 1); // Wait until PR1 is released

                __delay_ms(200); // Debounce Delay
                
                Input1++; // Increment Input 1
                sevenSeg(Input1, 0); // Display 1st Input
            }
            
            // Second Digit Input
            if (PORTBbits.RB2 == 1) // Photoresistor 2 is triggered
            {
                
                __delay_ms(200); // Debounce Delay

                while (PORTBbits.RB2 == 1); // Wait until PR2 is released

                __delay_ms(200); // Debounce Delay
                
                Input2++; // Increment Input 2
                sevenSeg(Input2, 1); // Display 2nd Input, with DP on to indicate Second Digit
            }
        }
    
    // Once Confirm Button is pressed
    PORTD = 0; // Clear Display

    FinalInput = (Input1*10)+Input2; // Moves input 1 to the 10s place and adds inputs 1 & 2 to get the code

}

void DetermineInput()
{
    if (Code == FinalInput) // Turn Correct LED ON for 3 Seconds if code is correct
    {
        PIR1bits.INT0IF = 0; // Clear the interrupt flag when done // Clear the interrupt flag for INT0
        PORTCbits.RC6 = 0; // Make sure Buzzer on RC6 is off
        
        PORTCbits.RC5 = 1;
        __delay_ms(3000);
        PORTCbits.RC5 = 0;
    }
    
    else // If Incorrect, turn on Buzzer for 1 second
    {
        PORTCbits.RC6 = 1;
        __delay_ms(1000);
        PORTCbits.RC6 = 0; 
    }
    
    FinalInput = 0; // Clear FinalInput to reset
    
}