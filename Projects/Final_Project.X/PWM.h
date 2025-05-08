/* 
 * File:   PWM.h
 * Author: student
 *
 * Created on May 1, 2023, 9:02 AM
 */

//------------------------------
// Definitions
//------------------------------

#define LED_PORT LATA

#ifndef PWM_H
#define	PWM_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

///////////////  TIMER 2
void TMR2_Initialize(void)
{
    // Set TMR2 to the options selected in the User Interface

    // T2CS FOSC/4; 
    T2CLKCON = 0x31;

    // T2PSYNC Not Synchronized; T2MODE Software control; T2CKPOL Rising Edge; T2CKSYNC Not Synchronized; 
    T2HLT = 0x00;

    // T2RSEL T2CKIPPS pin; 
    T2RST = 0x00;

    // PR2 255; 
    T2PR = 0xFF;

    // TMR2 0; 
    T2TMR = 0x00;

    // Clearing IF flag.
    PIR4bits.TMR2IF = 0;

    // T2CKPS 1:1; T2OUTPS 1:1; TMR2ON on; 
    T2CON = 0x80;
}

void TMR2_Start(void)
{
    // Start the Timer by writing to TMRxON bit
    T2CONbits.TMR2ON = 1;
}

void TMR2_StartTimer(void)
{
    TMR2_Start();
}

void TMR2_Stop(void)
{
    // Stop the Timer by writing to TMRxON bit
    T2CONbits.TMR2ON = 0;
}

void TMR2_StopTimer(void)
{
    TMR2_Stop();
}

uint8_t TMR2_Counter8BitGet(void)
{
    uint8_t readVal;

    readVal = TMR2;

    return readVal;
}

uint8_t TMR2_ReadTimer(void)
{
    return TMR2_Counter8BitGet();
}

void TMR2_Counter8BitSet(uint8_t timerVal)
{
    // Write to the Timer2 register
    TMR2 = timerVal;
}

void TMR2_WriteTimer(uint8_t timerVal)
{
    TMR2_Counter8BitSet(timerVal);
}

void TMR2_Period8BitSet(uint8_t periodVal)
{
   PR2 = periodVal;
}

void TMR2_LoadPeriodRegister(uint8_t periodVal)
{
   TMR2_Period8BitSet(periodVal);
}

///////////// END OF TIMER 


void PWM_Output_D8_Enable (void){
    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA; 
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS

    // Set RC2 as the output of CCP2
    RC2PPS = 0x0A; // 0x0A = CCP2 output

    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA; 
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS
}

void PWM_Output_D8_Disable (void){
    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA; 
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS

    // Set C2 as GPIO pin
    RC2PPS = 0x00;

    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA; 
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS
    
    TRISCbits.TRISC2 = 0;
}

void PWM2_Initialize(void)
{
    // Set the PWM2 to the options selected in the User Interface
	
	// MODE PWM; EN enabled; FMT left_aligned; 
	CCP2CON = 0x8C;  // enable / Right-aligned format  
	
	// RH 127; 
	CCPR2H = 0x01;    
	
	// RL 192; 
	CCPR2L = 0x9A;    

	// Selecting Timer 2
	CCPTMRS0bits.C2TSEL = 0x1;
    
}
void PWM2_LoadDutyValue(uint16_t dutyValue)
{
    dutyValue &= 0x03FF;

    // Load duty cycle value /Right-aligned format by default FMT=0
    if(CCP2CONbits.FMT)
    {
        dutyValue <<= 6;
        CCPR2H = dutyValue >> 8;
        CCPR2L = dutyValue;
    }
    else
    {
        CCPR2H = dutyValue >> 8;
        CCPR2L = dutyValue;
    }
}

 _Bool PWM2_OutputStatusGet(void)
{
    // Returns the output status
    return(CCP2CONbits.OUT);
}
 
 //------------------------------
// BUZZER TONE FUNCTIONS - PWM
//------------------------------

void playTone_CCP2(uint16_t frequency, uint16_t duration_ms) {
    uint16_t period = (_XTAL_FREQ / (frequency * 4)) - 1;
    if (period > 255) period = 255;

    T2PR = period;
    PWM2_LoadDutyValue(period / 8);  // ~12.5% duty - softer sound

    for (uint16_t i = 0; i < duration_ms; i++) {
        __delay_ms(1);
    }

    PWM2_LoadDutyValue(0);  // Silence buzzer after tone
    
}

void playButtonSound(uint8_t index) {
    switch (index) {
        case 0: playTone_CCP2(110, 150); break;  // A2
        case 1: playTone_CCP2(98, 150);  break;  // G2
        case 2: playTone_CCP2(87, 150);  break;  // F2
        case 3: playTone_CCP2(73, 150);  break;  // D2
    }
}

void playGameOver() {
    const uint16_t notes[] = { 262, 220, 196, 174, 147 }; // C4 -> D3 range
    const uint8_t count = sizeof(notes) / sizeof(notes[0]);

    for (uint8_t i = 0; i < count; i++) {
        uint16_t period = (_XTAL_FREQ / (notes[i] * 4)) - 1;
        if (period > 255) period = 255;

        T2PR = period;
        PWM2_LoadDutyValue(period / 6);  // 15?20% duty

        LED_PORT ^= 0x1F;  // Toggle all LEDs
        __delay_ms(300);

        LED_PORT = 0;
        PWM2_LoadDutyValue(0);  // Silence before next note
        __delay_ms(50);
    }
}