/*
 * IncFile1.h
 *
 * Created: 7/5/2021 15:31:26
 *  Author: jeron
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#include <avr/io.h>

typedef enum {IDLE, ING_CLAVE, ABIERTO, CLAVE_INC, M_HORA, M_CLAVE, M_CLAVE_F, M_CLAVE_E, M_CLAVE_N} MEF_state;
typedef enum {M_HORA_H,M_HORA_M,M_HORA_S} MEF_H_substate;

//Funciones generales de la MEF
void MEF_Init(uint32_t);
void MEF_Update (void);


#endif /* INCFILE1_H_ */