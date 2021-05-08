/*
 * seos.c
 *
 * Created: 08-May-21 4:39:28 PM
 *  Author: fedea
 */ 

#include "seos.h"
#include "timer.h"
#include "MEF.h"

volatile unsigned char Flag_MEF=0;
static unsigned char iCont=0;
static unsigned char iClock=0;

void SEOS_Init(){
	//configuro Timer0 para interrupciones cada 20 ms
	
	OCR0A = 156;
	TCCR0A = 0x02; //Seteo el timer0 en CTC
	TCCR0B = 0x05; //Uso preescalador en 1024
	TIMSK0 |= (1<<OCIE0A);	
}

void SEOS_Dispatch_Tasks(){
	
	if(Flag_MEF==1){ //Esto corre cada 100ms
		if(++iClock==10){
			TIMER_Update();
			iClock=0;
		}
		Flag_MEF=0;
		MEF_Update();
	}	
}

ISR(TIMER0_COMPA_vect){
	if(++iCont==5){
		Flag_MEF=1;	
		iCont=0;
	}
}