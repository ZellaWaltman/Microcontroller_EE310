;---------------------
; Title: Assignment 6, Seven Segment Counter
;---------------------
; Program Details:
; The purpose of this program is to 
    
; Inputs: 
; Outputs: 
; Date: Mar 11, 2025
; File Dependencies / Libraries: None 
; Compiler: xc8, 3.00
; Author: Zella Waltman
; Versions:
;       V1.0: 
; Useful links: 
;       Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
;       PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
;       List of Instructions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 


;---------------------
; Initialization - make sure the path is correct
;---------------------
#include "ConfigureFile.inc"
#include <xc.inc>

;---------------------
; Program Inputs
;---------------------
Inner_loop  equ 255 // in decimal
Outer_loop  equ 255
 
;---------------------
; Definitions
;---------------------
;#define SWITCH    LATD,2
;#define LED0      PORTD,1
;#define LED1	  PORTD,2

;---------------------
; Program Constants
;---------------------
REG10   equ     10h   // in HEX
REG11   equ     11h
		
;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld        ; Do not change
    
    ORG          0                ;Reset vector
    
    GOTO        _initialization

    ORG          20h           ; Begin assembly at 0020H

 _initialization: 
    RCALL _setupPortD
    RCALL _setupPortB
    GOTO _setupTBL
    
main:
    MOVFF TABLAT,PORTD
    RCALL DELAY
    
    MOVFF PORTB, 0x00 ; Load contents of PORTB into REG0
    
; IF only button A is pressed 
    MOVLW 0x01
    SUBWF 0x00,0 ; If REG0 = 0x01, only button A is pressed 
    BZ CountUP

; IF only button B is pressed     
    MOVLW 0x08
    SUBWF 0x00,0 ; If REG0 = 0x08, only button B is pressed 
    BZ CountDOWN
    
; IF BOTH buttons are pressed 
    MOVLW 0x09
    SUBWF 0x00,0 ; If REG0 = 0x09, both buttons are pressed
    BZ _setupTBL

; IF NO buttons are pressed: Do nothing, stay at the current counter value
   GOTO main

rst:
    MOVLW 0x00
    MOVWF TBLPTRL
    TBLRD* ; IF I HAVE TBLRD*, it will reset to 0 when no button is pressed, but if not, nothing happens when I press both buttons
    GOTO main
    
CountUP:
    MOVLW 0x0F
    CPFSEQ TBLPTRL ; if TBLPTRL addres is 0x0F, then skip next line and restart the count
    GOTO upAgain
    MOVLW 0x00
    MOVWF TBLPTRL
    TBLRD*
    GOTO main
upAgain:
    INCF TBLPTRL
    TBLRD*
    GOTO main

CountDOWN:
    MOVLW 0x00
    CPFSEQ TBLPTRL ; if TBLPTRL addres is 0x00, then skip next line and restart the count
    GOTO downAgain
    MOVLW 0x0F
    MOVWF TBLPTRL
    TBLRD*
    GOTO main
downAgain:
    DECF TBLPTRL
    TBLRD*
    GOTO main

DELAY:
    RCALL loop1
    RCALL loop1
    RCALL loop1
    RETURN
loop1:
    MOVLW       Inner_loop
    MOVWF       REG10,1
    MOVLW       Outer_loop
    MOVWF       REG11,1
loop2:
    DECF        REG10,1,0
    BNZ         loop2
    MOVLW       Inner_loop ; Re-initialize the inner loop for when the outer loop decrements.
    MOVWF       REG10,1
    DECF        REG11,1,0 // outer loop
    BNZ         loop2
    RETURN
    
; Initialize PortB: Button A and Button B
;-----------------------------------------
_setupPortB:
    BANKSEL PORTB
    CLRF PORTB ; Initialize PortB
    BANKSEL LATB
    CLRF LATB ; Initialize LATB
    BANKSEL ANSELB
    CLRF ANSELB ; Digital I/O
    BANKSEL TRISB
    MOVLW 0b00001001
    MOVWF TRISB ; PORTB0 and PORTB4 are inputs
    tblrd*
    RETURN

; Initialize PortD, Seven Segment Display
;-----------------------------------------
_setupPortD:
    BANKSEL PORTD
    CLRF PORTD ; Initialize PortD
    BANKSEL LATD
    CLRF LATD ; Initialize LATD
    BANKSEL ANSELD
    CLRF ANSELD ; Digital I/O
    BANKSEL TRISD
    MOVLW 0x00
    MOVWF TRISD ; PORTD is all outputs
    RETURN

; Initializing Table Pointer to begin at Address 200h
_setupTBL:
    MOVLW 0x00
    MOVWF TBLPTRL
    MOVLW 0x02
    MOVWF TBLPTRH
    MOVLW 0x00
    MOVWF TBLPTRU
    TBLRD*
    GOTO main
    
    ORG 200h ; Numbers are stored beginning at Address 200h
   
NUMBERS:
    DB 0x3F,0x30,0x5B,0x4F ; Numbers 0 through 3
    DB 0x66,0x6D,0x7D,0x07 ; Numbers 4 through 7
    DB 0x7F,0x67,0x77,0x7F ; Numbers 8 through B
    DB 0x39,0x3F,0x79,0x71 ; Numbers C through F