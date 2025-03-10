;---------------------
; Title: Assignment 4, Temperature Sensor
;---------------------
; Program Details:
; The purpose of this program is to create a heating and cooling control system.
; The desired temperature is set, and the system measures the actual temperature.
; If the environment temperature is higher than the reference temperature, the 
; cooling system will be turned on, PORTD2 will turn on, and the contReg will = 02h.
; If the environment temperature is less than the reference temperature, the
; heating systems will start, PORTD1 will turn on, and the contReg will = 01h.
; Otherwise, no light will turn on, and contReg will be 00h. Additionally, this
; program will convert the reference temperature and the measured temperature to
; decimal values and store these values in 3 registers, respectively.
    
; Inputs: measuredTempInput, refTempInput
; Outputs: PORTD
; Date: Mar 07, 2025
; File Dependencies / Libraries: None 
; Compiler: xc8, 3.00
; Author: Zella Waltman
; Versions:
;       V1.0: Original Program
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
#define	refTempInput 15
#define	measuredTempInput -5
 
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
contReg		equ     22h
maxRef		equ	00h
numerator	equ	01h
quotient	equ	02h
	
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
    MOVLW 0x00
    MOVWF contReg,1
    MOVFF WREG,STATUS
    MOVFF WREG,PORTD
    MOVFF WREG, ANSELD
    
    MOVLW 0b11111000 ; Hex 0xF8
    MOVWF TRISD,1 ; PORTD2, PORTD1, and PORTD0 are Outputs, rest are Inputs
    
    MOVLW 60
    MOVWF maxRef,1
    
    MOVLW measuredTempInput
    MOVWF measuredTemp,1
    GOTO DECIMAL_Meas
    
DECIMAL_Meas: ; Convert Measured Temp to Decimal
    MOVWF numerator,1 ; Place Measured Temp in REG01
    MOVLW 10
    CLRF quotient,1 ; Initialize quotient to 0
D3: 
    INCF quotient,1,0 ; Quotient increment
    SUBWF numerator,1,0 ; Subtract 10 from Measured Temp
    BC D3 ; If positive repeat subtraction
    ADDWF numerator,1,0 ; Add 10
    DECF quotient,1,0 ; One too many, decrease by 1
    MOVFF numerator, 0x70 ; Store first digit in REG70
    MOVFF quotient, numerator ; Place quotient in REG01
    CLRF quotient,1 ; Quotient = 0
D4: 
    INCF quotient,1,0 
    SUBWF numerator,1,0 ; Subtract 10
    BC D4 ; If positive repeat subtraction
    ADDWF numerator,1,0
    DECF quotient,1,0 ; One too many
    MOVFF numerator, 0x71 ; Place second digit in REG71
    MOVFF quotient, 0x72 ; Place last digit in REG72
    
    ; IF MEASURED TEMP IS NEGATIVE:
    ;---------------------------------
    ; -10 to -01 are F6h through FFh, much larger than the max reference value (60d) 
    CPFSGT maxRef,1 ; If measuredTemp < 60 (max refTemp), skip next line
    NEGF measuredTemp,1 ; If measuredTemp > 60 (3Ch), 2's Complement, sets Negative Flag = 1
    
    MOVLW refTempInput
    MOVWF refTemp,1
    GOTO DECIMAL_Ref
    
DECIMAL_Ref: ; Convert Reference Temp to Decimal
    MOVWF numerator,1 ; Place Ref Temp in REG01
    MOVLW 010
    CLRF quotient,1 ; Quotient = 0
D1: 
    INCF quotient,1,0
    SUBWF numerator,1,0 ; SUbtract 10 from Ref Temp
    BC D1 ; If positive repeat subtraction
    ADDWF numerator,1,0 
    DECF quotient,1,0
    MOVFF numerator, 0x60 ; Store first digit in REG60
    MOVFF quotient, numerator 
    CLRF quotient,1 ; Quotient = 0
D2: 
    INCF quotient,1,0
    SUBWF numerator,1,0
    BC D2 ; If positive repeat subtraction
    ADDWF numerator,1,0
    DECF quotient,1,0
    MOVFF numerator, 0x61 ; Store second digit in REG61
    MOVFF quotient, 0x62 ; Store last digit in REG62
    
    ; TEST if measured is greater by subtracting measuredTemp - refTemp & check STATUS
    SUBWF measuredTemp,0,0
    
    ; SPECIAL CASE: IF MEASURED TEMP IS NEGATIVE:
    ;---------------------------------------------
    BTFSC STATUS,4,0 ; If Negative bit (bit 4) is 0, skip this next line
    GOTO LED_COOL ; measuredTemp < refTemp, Go to LED_COOL,
    
    ; IF Statements
    ;---------------
    
    ; If measuredTemp = refTemp, Zero bit (bit 2) will be 1.
    BTFSC STATUS,2,0 ; If Zero bit (bit 2) is 0, skip this next line
    GOTO LED_OFF
    
    ; If measuredTemp > refTemp, Carry bit will be set (Carry = 1).
    BTFSC STATUS,0,0 ; If Carry bit (bit 0) is 0, skip this next line
    GOTO LED_HOT
    
    GOTO LED_COOL ; Otherwise, go to LED_COOL, measuredTemp < refTemp
      
LED_OFF:
    MOVLW 0x00
    MOVWF contReg,1
    
    MOVLW 0x00;
    MOVFF WREG,PORTD
    
    GOTO _END
    
LED_COOL:
    MOVLW 0x01
    MOVWF contReg,1
    
    MOVLW 0x02
    MOVFF WREG,PORTD
    
    GOTO _END
    
LED_HOT:
    MOVLW 0x02
    MOVWF contReg,1
    
    MOVLW 0x04
    MOVFF WREG,PORTD
    
    GOTO _END
    
_END:   
END