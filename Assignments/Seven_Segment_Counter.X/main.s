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
;#include "C:\Users\student\Documents\myMPLABXProjects\ProjectFirstAssemblyMPLAB\FirstAssemblyMPLAB.X\ConfigureFile.inc"

#include <xc.inc>

;---------------------
; Program Inputs
;---------------------
#define	refTempInput 10
#define	measuredTempInput 50
 
;---------------------
; Definitions
;---------------------
#define SWITCH    LATD,2
#define LED0      PORTD,1
#define LED1	  PORTD,2

;---------------------
; Program Constants
;---------------------
refTemp		equ     20h 
measuredTemp	equ     21h
	
;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld        ; Do not change
    
    ORG          0                ;Reset vector
    
    GOTO        _start

    ORG          0020H           ; Begin assembly at 0020H

_start:
    
    ; Clearing STATUS REG, contREG, and PortD
    ;-----------------------------------------
    BANKSEL PORTD
    CLRF PORTD,1 ; Initialize PortD
    BANKSEL LATD
    CLRF LATD,1 ; Initialize LATD
    BANKSEL ANSELD
    CLRF ANSELD,1 ; Digital I/O
    BANKSEL TRISD
    CLRF LATD,1 ; Initialize LATD
    MOVLW 0x00
    MOVWF TRISD,1 ; All Ports are Outputs
    BANKSEL 0
    
    