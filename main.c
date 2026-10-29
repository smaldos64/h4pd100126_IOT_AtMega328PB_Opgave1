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
#include "include/Timer/Timer.h"
#include "include/Timer/Timer0.h"
#include "include/UART/Uart.h"

void ConvertReceivedCharMain(char *ReceivedChar)
{
	// Den smarte måde at få konverteret små bogstaver om til store bogstaver og
	// modsat er ved brug af Xor, som vi tidligere har set. Så kan vi klare det i
	// én linje kode.
	
	if ( ((*ReceivedChar >= 0x41) && (*ReceivedChar <= 0x5D)) ||
	     ((*ReceivedChar >= 0x61) && (*ReceivedChar <= 0x7D)))
	{
		*ReceivedChar = *ReceivedChar ^ Upper_Lower_Bit_Value;
	}
}


int main(void)
{
	DDRB = 0xFF;
	PORTB = 0;
		
	Setup_Timer0_Overflow_Interrupt(VariableValue1SecValue8Bit);
	Enable_Timer0_Overflow_Interrupt();

	RS232Init();
	SetupUARTFunctionCallbackPointer(ConvertReceivedCharMain);
	Enable_UART_Receive_Interrupt();

	sei();
	
    /* Replace with your application code */
    while (1) 
    {
		
    }
}

