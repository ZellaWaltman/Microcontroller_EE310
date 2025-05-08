/*
 * ------------------------------------------------
 * Simon Says Game: Functions Header File
 * --------------------------------------------------
 *   This is a header file which contains functions for the main.asm file
 * These functions include Functions for the LCD display as well as Game functions
 * such as a function for flashing the LED, adding a step to the sequence,
 * showing the sequence to the user, and checking the user's input.
 */

//------------------------------
// Definitions
//------------------------------

#define RS LATB0
#define E LATB1

#define BUTTONS PORTC

//------------------------------
// Initialization of Variables
//------------------------------

char Level[50];
char Score[50];

uint8_t sequence[32]; // Random sequence user must follow
uint8_t seqLength = 0; // Current length of the game sequence

//------------------------------
// LCD Function Declarations
//------------------------------

void InitializeLCD();
void LCD_Command(char);
void LCD_Position(char, char, const char *);
void LCD_Char(char);
void LCD_String(const char *);

//------------------------------
// LCD Functions
//------------------------------

void InitializeLCD()
{
    __delay_ms(15); // 15ms, 16x2 LCD Power on delay
    TRISD = 0x00; // Set PORTD as output PORT for LCD data(D0-D7) pins
    TRISB = 0x00; // Set PORTB as output PORT LCD Control(RS,EN) Pins
    LCD_Command(0x01); // clear display screen
    LCD_Command(0x38); // uses 2 line and initialize 5*7 matrix of LCD
    LCD_Command(0x0c); // display on cursor off
    LCD_Command(0x06); // increment cursor (shift cursor to right)
}

void LCD_Command(char command)
{
    LATD = command; // Send data to PORT as a command for LCD 
    RS = 0; // Command Register is selected
    E = 1; // High-to-Low pulse on Enable pin to latch data
    NOP();
    E = 0;
    __delay_ms(6);
}

void LCD_Clear()
{
        LCD_Command(0x01); /* clear display screen */
}

void LCD_Char(char data)
{
    LATD = data;            /* Send data to LCD */  
    RS = 1;                /* Data Register is selected */
    E = 1;                  /* High-to-Low pulse on Enable pin to latch data */   
    NOP();
    E = 0;
    __delay_ms(2);
}

void LCD_String(const char *message)
{
    while((*message)!='\0')
    {       
      LCD_Char(*message);
      message++;    
    }
}

void LCD_Position(char row,char pos,const char *message)
{
    char location = 0;
    if(row<=1)
    {
        location = (0x80) | ((pos) & 0x0f); /*Print message on 1st row and desired location*/
        LCD_Command(location);
    }
    else
    {
        location=(0xC0) | ((pos) & 0x0f); /*Print message on 2nd row and desired location*/
        LCD_Command(location);    
    } 
    
    LCD_String(message);
}

//------------------------------
// Game Functions
//------------------------------

void flash_led(uint8_t index) {
    LATA = (1 << led_pins[index]);  // Turns on 1 LED by shifting 1 to the correct bit based on index
    playButtonSound(index);   // Play tone associated with button
    __delay_ms(400);
    LATA = 0; // Turn off all LEDs
    __delay_ms(200);
}

void show_sequence() { // Plays Current Sequence for the User
   
    LCD_Clear();
    LCD_Position(1,0,"Watch Carefully!");
    __delay_ms(1000);
    
    for (uint8_t i = 0; i < seqLength; i++) {
        flash_led(sequence[i]);
    }
}

void add_step() {
    sequence[seqLength++] = rand() % 4; // Adds a new random LED to the sequence
}

bool check_input() { // Check if User Input matches the Sequence
    for (uint8_t i = 0; i < seqLength; i++) { // Goes through each part of sequence
        if (userInput[i] != sequence[i]) { // If User Input does not match -> False
            return false;
        }
    }
    
    return true; // If User Input Matches Sequence
    
}