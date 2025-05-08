/*
 * ---------------------
 * Title: Simon Says Game
 * ---------------------
 * Program Details:
 *      This program is designed to be a Simon Says type game. When the player
 * presses start, an LED will light up and a sound will play. Each time the user
 * correctly matches the sequence, another LED will be added, and the score will
 * increase. This will continue until the user inputs the incorrect sequence, upon
 * which a tone will play and the game will start over on the Start Screen. The
 * system utilizes 4 LEDs, an LCD which will output the Start, Game Over, and
 * current score, as well as 4 buttons which utilize the PORT C Interrupt-on-Change
 * peripheral. A buzzer utilizes PWM in order to play the distinct tones associated 
 * with each LED.
 * 
 * Inputs: RC3-RC6 (IOC Interrupt Buttons)
 *   IOC Interrupt Button Mapping:
 *      RC3 - Button 1, corresponds with LED 0
 *      RC4 - Button 2, corresponds with LED 1
 *      RC5 - Button 3, corresponds with LED 2
 *      RC6 - Button 4, corresponds with LED 3
 * 
 * Outputs: PORTA (LEDS), PORTB (LCD RS and E), PORTD (LCD Data), RC2 (Buzzer)
 * PORTD Mapping (LCD Display Data Pins D1-D7):
 *   RA0 - LED 0 (RED)
 *   RA1 - LED 1 (YELLOW)
 *   RA3 - LED 2 (GREEN)
 *   RA4 - LED 3 (BLUE)
 * 
 * PORTB Mapping:
 *   RB0 - LCD Register Select Pin
 *   RB1 - LCD Enable Pin
 * 
 * PORTD Mapping (LCD Display Data Pins D1-D7):
 *   RD0 - D0
 *   RD1 - D1
 *   RD2 - D2
 *   RD3 - D3
 *   RD4 - D4
 *   RD5 - D5
 *   RD6 - D6
 *   RD7 - D7
 * 
 * Setup: C-Simulator
 * Date: May 8th, 2025
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File, Functions Header File, and Initialization Header File 
 * Compiler: xc8, 3.0
 * Author: Zella Waltman
 * Versions:
 *      V1.0: Original
 * Useful links:  
 *      Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
 *      PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
 *      List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 
 */

#include <xc.h>
#include "ConfigureFile.inc"
#include "Initialization.h"
#include "Functions.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//-------------------------
// Main Function
//-------------------------

void main() {
    
    //--------------------------    
    // CLEARING & INITIALIZING
    //--------------------------   
    
    seqLength = 0;  
    userIndex = 0;
    memset(userInput, 0xFF, sizeof(userInput)); // clear old values
    
    InitializePorts();
    InitializeLCD();
    INTERRUPT_Initialize(); // Interrupt Initialize
    
    // PWM Initialization
    PWM_Output_D8_Enable();
    TMR2_Initialize();
    PWM2_Initialize();
    PWM2_LoadDutyValue(0); // Silence Buzzer at startup

    //-----------------------
    // START SCREEN
    //-----------------------    
    
    while (1) { // BEFORE GAME STARTS. START SCREEN.
        
        LCD_Clear();
        LCD_Position(1, 0, "Press Any Button");
        LCD_Position(2, 0, "   To Start!    ");

        // Wait for interrupt to set flag. Until button is pressed, stay on Start Screen
        // LEDs will flash in sequence to be more engaging to the audience while on the Start Screen
        int direction = 1;
        int i = 0;

        while (!gameStarted) {
            LATA = (1 << led_pins[i]);  // Light one LED
            __delay_ms(75);
            LATA = 0;
            __delay_ms(50);

            i += direction;

            if (i == 3 || i == 0) {
                direction = -direction;  // Reverse at ends

            if (gameStarted) break;     // Exit immediately if button pressed
            }
        }   
        
        __delay_ms(200); // Debounce
        
        // After Start Button Pressed, Initialize Sequence:
        seqLength = 0;
        add_step(); // Add a random step to begin. Will add more to the sequence in check_input sequence

        userIndex = 0;
        memset(userInput, 0xFF, sizeof(userInput));  // Clear previous input
        
        //-----------------------
        // MAIN GAME LOOP
        //-----------------------
        
        while (1) { 

            // Add step and show it
            userIndex = 0;
            show_sequence();    // Show it to user

            // Prompt for input
            LCD_Clear();
            LCD_Position(1,0,"Your Turn!");

            // Wait for user to finish input
            while (userIndex < seqLength);

            // Check result
            if (check_input()) { // If Sequence Is Correct (Input = True)

                LCD_Clear();
                LCD_Position(1,0,"Correct!");

                // This is used to convert integer value to ASCII string
                sprintf(Level, "Level: %u", seqLength);
                LCD_Position(2,0, Level); // Print Level the User is On

                __delay_ms(1000);
                
                add_step();  // Add new step after correct sequence input

            } 

            else {
                
                playGameOver();
                
                LCD_Clear();
                LCD_Position(1,0,"Game Over");

                // This is used to convert integer value to ASCII string
                // seqLength will be number of Current Round, so Current Round must be Removed
                sprintf(Score, "Level: %u", (seqLength - 1));

                LCD_Position(2,0, Score); // Print Final Score

                // Reset
                seqLength = 0;
                LED_PORT = 0;
                __delay_ms(4000);
                
                gameStarted = false;
                
                break;

            }
        }
    }
}