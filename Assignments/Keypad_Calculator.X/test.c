#include <xc.h> // must have this
#include <stdlib.h>
//#include "../../../../../Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h"
#include <pic18f47k42.h>
#include "ConfigureFile.inc"

//#include "C:\Program Files\Microchip\xc8\v2.40\pic\include\proc\pic18f46k42"

void calc(void);    //Performs all calculator readings, does operations, and displays
void MSDelay(unsigned int,unsigned int,unsigned int); //Delay function
int Getkey(void); //Reads keypad

int keypad[4][4] = {0,1,2,3, 4,5,6,7, 8,9,10,11, 12,13,14,15};

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4



void main (void){
    int digit1=0,digit2=0,Operation_REG,X_Input_Reg,Y_Input_Reg,Display_Result_REG,reset;
    ANSELD = 0;
    ANSELB = 0;
    TRISB = 0b00000000;
    TRISD = 0x0F;
    PORTD = 0x0;
    PORTB = 0b00000000;
    while(1)
    {
       // MSDelay(2,15000,40000);
        reset = 0;
        digit1 = 0;
        digit2 = 0;
        Operation_REG = 0;
        X_Input_Reg = 0;
        Y_Input_Reg = 0;
        Display_Result_REG = 0;
        
        calc();
        
        
    }
}

int Getkey(void)
{
    PORTD = 0x01;           //powers first row and checks for high output
  //  // MSDelay(1,150,100);     
    if(PORTD == 0x11) return keypad[0][1];
    if(PORTD == 0x21) return keypad[0][2];
    if(PORTD == 0x41) return keypad[0][3];
    if(PORTD == 0x81) return keypad[2][2];
    PORTD = 0x02;           //powers second row and checks for high output
    // MSDelay(1,150,100);
    if(PORTD == 0x12) return keypad[1][0];
    if(PORTD == 0x22) return keypad[1][1];
    if(PORTD == 0x42) return keypad[1][2];
    if(PORTD == 0x82) return keypad[2][3];
    PORTD = 0x04;           //powers third row and checks for high output
    // MSDelay(1,150,100);
    if(PORTD == 0x14) return keypad[1][3];
    if(PORTD == 0x24) return keypad[2][0];
    if(PORTD == 0x44) return keypad[2][1];
    if(PORTD == 0x84) return keypad[3][0];
    PORTD = 0x08;           //powers fourth row and checks for high output
    // MSDelay(1,150,100);
    if(PORTD == 0x18) return keypad[3][3];
    if(PORTD == 0x28) return keypad[0][0];
    if(PORTD == 0x48) return keypad[3][2];
    if(PORTD == 0x88) return keypad[3][1];
    else return -1;
}

void MSDelay(unsigned int itime1,unsigned int itime2,unsigned int itime3)
{
    unsigned int i,j,k;
    for (i=0; i<itime1; i++)
    {
    for(j=0; j<itime2; j++);
    for(k =0;k<itime3; k++);
    }
}

void calc(void)
{
int digit1=0,digit2=0,Operation_REG,X_Input_Reg,Y_Input_Reg,Display_Result_REG,reset;
while(1)
        {
        if(PORTD != 0x0F) //checks for key press
        {
      // MSDelay(1,150,100);   
      digit1 = Getkey();    //gets first key
        }
      if(digit1 >= 10) PORTB = 0;   //checks if valid digit
        if(digit1>0 && digit1< 10){
          PORTB = digit1;
          break;
        }
        }
      //   MSDelay(2,15000,40000);
        while(1 && reset == 0)  //start of 2nd digit loop
        {
        if(PORTD != 0x0F)
        {
      // MSDelay(1,150,100);
      digit2 = Getkey();    //gets 2nd digit
       
      if(digit2 == 15)  //checks if reset was pressed
      {
          reset = 1;
          PORTB = 0;
          break;
      }
      if(digit2 >= 10)  //checks if operation is pressed.
      {
          Operation_REG = digit2;
          X_Input_Reg = digit1;
          break;
      }
        if(digit2 >= 0 && digit2 <10) { //checks if digit is pressed
          digit1 *=10;
          X_Input_Reg = digit1 + digit2;
          PORTB = X_Input_Reg;
         
          break;
        }
        }
        }
       
     //  MSDelay(2,15000,40000);
       while(1 && reset == 0)   //start of 3rd reading loop, loops until operation
        {
        if(PORTD != 0x0F)
        {
      // MSDelay(1,150,100);
      digit2 = Getkey();    //gets key
      
      if(digit2 == 15)  //was reset pressed?
      {
          reset = 1;
          PORTB = 0;
          break;
      }
      if(digit2 >= 10 && digit2 <= 13)  //checks for operation
      {
          PORTB = 0x80;
          Operation_REG = digit2;
          break;
      }
        }
        }
        
      // MSDelay(2,15000,40000);
       while(1 && reset == 0)   //4th input loop
        {
        if(PORTD != 0x0F)
        {
      // MSDelay(1,150,100);
      digit1 = Getkey();    //gets 4th input
      
      if(digit1 == 15)  //is reset pressed?
      {
          reset = 1;
          PORTB = 0;
          break;
      }  
      if(digit1 >= 10) PORTB = 0x80;    //anything other than digit does nothing
      
        if(digit1 >= 0 && digit1 <10) { //reads valid digit and breaks loop
          PORTB = digit1;
          break;
        }
        }
        }
       
     //  MSDelay(2,15000,40000);
        while(1 && reset == 0)  //enter fifth reading loop
        {
        if(PORTD != 0x0F)
        {
      // MSDelay(1,150,100);
      digit2 = Getkey();
      if(digit2 == 15)  //is reset pressed?
      {
          reset = 1;
          PORTB = 0;
          break;
      }
        if(digit2 >= 0 && digit2 <10) { //checks for 2nd valid digit
          digit1 *=10;
          Y_Input_Reg = digit1 + digit2;
          PORTB = Y_Input_Reg;
          break;
        }
        
      if(digit2 == 14)  //computes value if 2nd number is only 1 digit
      {
          Y_Input_Reg = digit1;
        switch(Operation_REG){  //reads Operation_REG and performs correct op.
                case 10: Display_Result_REG = X_Input_Reg + Y_Input_Reg;
                break;
                 case 11: Display_Result_REG = X_Input_Reg - Y_Input_Reg;
                break;
                case 12: Display_Result_REG = X_Input_Reg * Y_Input_Reg;
                break;
                case 13: Display_Result_REG = X_Input_Reg / Y_Input_Reg;
                break;  
        }
                PORTB = Display_Result_REG;
                break;                
        }
        }
        }
       
     //  MSDelay(2,15000,40000);
       while(1 && reset == 0){  //final reading loop, must be equal sign or reset
           if(PORTD != 0x0F)
           {
               // MSDelay(1,150,100);
               digit2 = Getkey();
               if(digit2 == 15) //is it reset?
                {
                reset = 1;
                PORTB = 0;
                break;
                 }
               if(digit2 == 14) //did they press equals?
               {
            switch(Operation_REG){ //reads Operation_REG and performs correct op.
                case 10: Display_Result_REG = X_Input_Reg + Y_Input_Reg;
                PORTB = Display_Result_REG;
                break;
                 case 11: Display_Result_REG = X_Input_Reg - Y_Input_Reg;
                 PORTB = Display_Result_REG;
                break;
                case 12: Display_Result_REG = X_Input_Reg * Y_Input_Reg;
                PORTB = Display_Result_REG;
                break;
                case 13: Display_Result_REG = X_Input_Reg / Y_Input_Reg;
                PORTB = Display_Result_REG;
                break;  
        }
               }
                
       }
       }
}