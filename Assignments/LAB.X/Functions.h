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

void sevenSeg(int index, int secondNumInput);

void Count(void);

//------------------------------
// Initialization of Variables
//------------------------------

int Number = 0;

//---------------------------------
// Setting up Seven Segment Values
//---------------------------------
unsigned char sevenSegValues[] = {0x3F,0x30,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7F,0x39,0x3F,0x79,0x71};

//------------------------------
// Functions
//------------------------------

void sevenSeg(int index, int secondNumInput) // Seven Segment Display
{
    if ( index < 0 || index > 15 )
    {
        if (PORTBbits.RB0 == 1) // Button 1 is pressed
            {
                
                index = 0; // Restart Count
                
            }
            
            if (PORTBbits.RB1 == 1) // Button 2 is pressed
            {
                
                index = 15; // Restart Count
                
            }
    }
    
    PORTD = sevenSegValues[index]; // Output value on seven segment based on index number
    
}

//-----------------------------
// Read and Check User Inputs
//-----------------------------
void Count()
{
    Number = 0;
   
        while (1)
        {
           
            // First Digit Input
            if (PORTBbits.RB0 == 1) // Button 1 is pressed
            {
                __delay_ms(1000); // 1 second delay
                
                Number++; // Increment Number by 2
                sevenSeg(Number, 0); // Display Number
            }
            
            // Second Digit Input
            if (PORTBbits.RB1 == 1) // Button 2 is pressed
            {
                
                __delay_ms(1000); // 1 second delay
                
                Number++; // Decrement Number by 2
                sevenSeg(Number, 1); // Display 2nd Input, with DP on to indicate Second Digit
            }
        }
}