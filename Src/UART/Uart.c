/*
 * Uart.c
 *
 * Created: 15-01-2018 09:44:48
 *  Author: ltpe
 */ 


#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../Include/ProjectDefines.h"
#include "../../Include/UART/Uart.h"
 //#include "../../Include/Main.h"

 static FILE uart_output = FDEV_SETUP_STREAM(uart_putch, NULL, _FDEV_SETUP_WRITE);
 static FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getch, _FDEV_SETUP_READ);
 
void ConvertReceivedChar(char *ReceivedChar)
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

 void RS232Init(void)
 {
	 // Set baud rate
	 UBRR1L = (unsigned char)(MyUBRR & 0xff);
	 UBRR1H = (unsigned char)(MyUBRR>>8);
	 #ifdef UART_NORMAL_MODE
	 UCSR1A = 0x00;
	 #else
	 UCSR1A = 1<<U2X; // Set *2 mode
	 #endif
	 UCSR1B = (1<<RXEN1) | (1<<TXEN1);  //0x18 , enable receive and transmit
	 UCSR1C =  (1<<UCSZ11) | (1<<UCSZ10); // 8 data bit 1 stop bit
	 stdout = &uart_output;
	 stdin = &uart_input;
 }

 void Enable_UART_Receive_Interrupt()
 {
	 UCSR1B |= (1 << RXCIE1); // Enable the USART Recieve Complete interrupt (USART_RXC)
 }

 void Disable_UART_Receive_Interupt()
 {
	UCSR1B &= ~(1 << RXCIE1); // Disable the USART Recieve Complete interrupt (USART_RXC)
 }

 int uart_getch(FILE* stream)
 {
	 while( !(UCSR1A & (1<<RXC1)))  ; // do nothing but wait
	 return( UDR1);
 }

 int uart_putch(char ch, FILE* stream)
 {
	 while( !(UCSR1A & (1<<UDRE1))) ;
	 UDR1 = ch;
	 return 0;
 }

 ISR(USART1_RX_vect)
 {
	 char ReceivedByte;
	 ReceivedByte = UDR1;
	 ConvertReceivedChar(&ReceivedByte);
	 UDR1 = ReceivedByte; // Echo back the received byte to the computer
}