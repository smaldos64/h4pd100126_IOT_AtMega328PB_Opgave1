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

 static FILE uart_output = FDEV_SETUP_STREAM(uart_putch, NULL, _FDEV_SETUP_WRITE);
 static FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getch, _FDEV_SETUP_READ);

 /* En hjhælper makro er nødvendig for at ekspandere AVR8_UART_NUMBER macro til det aktuelle nummer,
   hviket skyldes, at makro argumenter brugt med ## operatoren ikke er ekspanderet, så 
   vi kan ikke overføre AVR8_UART_NUMBER direkte til denne makro. Vi må i stedet for bruge den næste 
   makro - Denne makro ekspandere AVR8_UART_NUMBER til dens rette værdi. */
#define 	MAKE_UART_NAME_(name1, number, name2) name1 ## number ## name2
/* create name of the register or bit based on */
#define		MAKE_UART_NAME(name1 , num, name2 ) MAKE_UART_NAME_(name1, num, name2 )

/* Create the actual registry and bit names for UART */
#define 	UCSRA	MAKE_UART_NAME(UCSR, AVR8_UART_NUMBER, A)
#define 	UCSRB	MAKE_UART_NAME(UCSR, AVR8_UART_NUMBER, B)
#define		UCSRC	MAKE_UART_NAME(UCSR, AVR8_UART_NUMBER, C)
#define		UBRRH	MAKE_UART_NAME(UBRR, AVR8_UART_NUMBER, H)
#define		UBRRL	MAKE_UART_NAME(UBRR, AVR8_UART_NUMBER, L)
#define		RXEN	MAKE_UART_NAME(RXEN, AVR8_UART_NUMBER, )
#define		TXEN	MAKE_UART_NAME(TXEN, AVR8_UART_NUMBER, )
#define		UCSZ0	MAKE_UART_NAME(UCSZ, AVR8_UART_NUMBER, 0)
#define		UCSZ1	MAKE_UART_NAME(UCSZ, AVR8_UART_NUMBER, 1)
#define		RXCIE	MAKE_UART_NAME(RXCIE, AVR8_UART_NUMBER, )
#define		UDRE	MAKE_UART_NAME(UDRE, AVR8_UART_NUMBER, )
#define		UDR		MAKE_UART_NAME(UDR, AVR8_UART_NUMBER, )
#define		RXC		MAKE_UART_NAME(RXC, AVR8_UART_NUMBER, )
#define		U2X		MAKE_UART_NAME(U2X, AVR8_UART_NUMBER, )

/* Define the USART vector name */
/* the other MCUs have USART number in the ISR name */
#define	MAKE_UART_VECT_(number)		USART ## number ## _RX_vect
#define	MAKE_UART_VECT(num)			MAKE_UART_VECT_(num)
#define	UART_ISR_VECTOR	MAKE_UART_VECT(AVR8_UART_NUMBER)	


 
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

static Function_Pointer_With_One_CharPointer_Parameter Callback_Function_Pointer = NULL;

void SetupUARTFunctionCallbackPointer(Function_Pointer_With_One_CharPointer_Parameter 
                                      Function_To_Callback)
{
	Callback_Function_Pointer = Function_To_Callback;
}


 void RS232Init(void)
 {
	 // Set baud rate
	UBRRL = (unsigned char)(MyUBRR & 0xff);
	UBRRH = (unsigned char)(MyUBRR>>8);
	#ifdef UART_NORMAL_MODE
	UCSRA = 0x00;
	#else
	UCSRA = 1<<U2X0; // Set *2 mode
	#endif
	UCSRB = (1<<RXEN) | (1<<TXEN);  //0x18 , enable receive and transmit
	UCSRC =  (1<<UCSZ1) | (1<<UCSZ0); // 8 data bit 1 stop bit

	 stdout = &uart_output;
	 stdin = &uart_input;
 }

 void Enable_UART_Receive_Interrupt()
 {
	 UCSRB |= (1 << RXCIE); // Enable the USART Recieve Complete interrupt (USART_RXC)
 }

 void Disable_UART_Receive_Interupt()
 {
	UCSRB &= ~(1 << RXCIE); // Disable the USART Recieve Complete interrupt (USART_RXC)
 }

 int uart_getch(FILE* stream)
 {
	 while( !(UCSRA & (1<<RXC)))  ; // do nothing but wait
	 return( UDR);
 }


 int uart_putch(char ch, FILE* stream)
 {
	 while( !(UCSRA & (1<<UDRE)))  ; // do nothing but wait
	 UDR = ch;
	 return(ch);
 }
 

 ISR(UART_ISR_VECTOR)
 {
	 char ReceivedByte;
	 ReceivedByte = UDR;
	 ConvertReceivedChar(&ReceivedByte);
	 UDR = ReceivedByte; // Echo back the received byte to the computer
}