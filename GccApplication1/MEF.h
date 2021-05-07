/*
 * IncFile1.h
 *
 * Created: 7/5/2021 15:31:26
 *  Author: jeron
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

typedef enum {IDLE, ING_CLAVE, ABIERTO, CLAVE_INC, M_HORA, M_CLAVE, M_CLAVE_F, M_CLAVE_E, M_CLAVE_N} MEF_state;

void MEF_update (void);
void changeING_CLAVE (void);
void Out_IngClave (void);
void ChangeABIERTO(void);
void ChangeCLAVE_INC(void);
void OutClaveInc(void);
void ChangeIDLE(void);

#endif /* INCFILE1_H_ */