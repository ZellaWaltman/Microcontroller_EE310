/*
 * ------------------------------------------------
 * ADC and LCD: Functions Header File
 * --------------------------------------------------

 * This is a header file which contains functions for the main.asm file
 * These functions include Functions for the LCD display.
 */

//------------------------------
// Definitions
//------------------------------

#define RS LATB0
#define EN LATB1

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
    EN = 1; // High-to-Low pulse on Enable pin to latch data
    NOP();
    EN = 0;
    __delay_ms(6);
}

void LCD_Char(char data)
{
    LATD = data;            /* Send data to LCD */  
    RS = 1;                /* Data Register is selected */
    EN = 1;                  /* High-to-Low pulse on Enable pin to latch data */   
    NOP();
    EN = 0;
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