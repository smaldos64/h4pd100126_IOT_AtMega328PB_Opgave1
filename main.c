/*
 * Timer_Interrupt_C.c
 *
 * Created: 20-04-2022 14:53:40
 * Author : ltpe
 */ 

#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#include "include/ProjectDefines.h"
#include "include/Timer.h"
#include "include/Timer0.h"


int main(void)
{
	DDRB = 0xFF;
	PORTB = 0;
	
	Setup_Timer0_Overflow_Interrupt(VariableValue1SecValue8Bit);
	Enable_Timer0_Overflow_Interrupt();
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

