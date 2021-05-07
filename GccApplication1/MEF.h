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

//Funciones generales de la MEF
void MEF_Update (void);

//Funciones de cambios de estados
void changeING_CLAVE (void);
void ChangeABIERTO(void);
void ChangeCLAVE_INC(void);
void ChangeIDLE(void);
void ChangeM_CLAVE(void);
void ChangeM_CLAVE_N (void);
void ChangeM_CLAVE_F(void);
void ChangeM_CLAVE_E(void);

//Funciones de salida de la MEF
void Out_IngClave (void);
void OutClaveInc(void);
void OutAbierto(void);
void OutIdle(void);
void OutMClave(void);
void OutMClaveN(void);

#endif /* INCFILE1_H_ */