;---------------------
; Initialization
;---------------------
#include "ConfigureFile.inc"
#include <xc.inc>

;---------------------
; Program Inputs
;---------------------
Inner_loop  equ 255 // in decimal
Outer_loop  equ 130
 
;---------------------
; Program Constants
;---------------------
REG10   equ     10h   // in HEX
REG11   equ     11h

;---------------------
; Definitions
;---------------------
#define SWITCH    PORTD,1  
#define LED0      PORTD,0
#define LED1      PORTB,0
   
;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld        ; Do not change
    
    ORG          0                ;Reset vector
    
    MOVLW 0
    MOVFF WREG,STATUS
    MOVLW 96
    ADDLW 70


