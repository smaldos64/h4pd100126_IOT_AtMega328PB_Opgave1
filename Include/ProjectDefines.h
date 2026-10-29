/*
 * ProjectDefines.h
 *
 * Created: 15-01-2018 14:56:37
 *  Author: ltpe
 */ 

#include <avr/io.h>

#ifndef PROJECTDEFINES_H_
#define PROJECTDEFINES_H_

#if defined (_AVR_IOM168_H_) || defined (_AVR_IOM328P_H_)
#define USR0_Vect_Num USART_RX_vect

#endif

#if defined (_AVR_IOM2560_H_) || defined (_AVR_ATMEGA328PB_H_INCLUDED)
#define USR0_Vect_Num USART0_RX_vect 

#endif

typedef void (*Function_Pointer_With_One_CharPointer_Parameter)(char *);

#if defined(_AVR_IOM2560_H_) || defined (_AVR_ATMEGA328PB_H_INCLUDED)
#define GreenLedBit		1
#define YellowLedBit	2
#define RedLedBit		3
#endif

#define F_CPU 16000000UL

#define Upper_Lower_Bit_Position 5
#define Upper_Lower_Bit_Value (1 << Upper_Lower_Bit_Position)

#endif /* PROJECTDEFINES_H_ */