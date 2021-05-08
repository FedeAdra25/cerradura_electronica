/*
 * seos.h
 *
 * Created: 08-May-21 4:39:43 PM
 *  Author: fedea
 */ 

#ifndef SEOS_H_
#define SEOS_H_
#include <avr/io.h>
#include <avr/interrupt.h>


void SEOS_Init();
void SEOS_Dispatch_Tasks();



#endif /* SEOS_H_ */