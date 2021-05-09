/*
 * timer.h
 *
 * Created: 08-May-21 6:03:27 PM
 *  Author: fedea
 */ 


#ifndef TIMER_H_
#define TIMER_H_
#include <avr/io.h>


void TIMER_Init();
void TIMER_Update();

unsigned char* TIMER_GetHora();

void TIMER_ModificarHora(uint8_t);
void TIMER_ModificarMinutos(uint8_t);
void TIMER_ModificarSegundos(uint8_t);



#endif /* TIMER_H_ */