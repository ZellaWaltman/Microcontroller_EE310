/*
 * File:   main.c
 * Author: zellwaltman
 *
 * Created on May 4, 2025, 3:09 PM
 */


#include <xc.h>
#include "ConfigureFile.inc"
#include "Functions.h"
#include "Initialization.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//------------------------------
// Definitions
//------------------------------


//-------------------------
// Main Function
//-------------------------

void main() {
    initialization(); // Initialize everything, located in "Initialization.h"
    
    while(1)
    {
        
        Count(); // Count
    }
}