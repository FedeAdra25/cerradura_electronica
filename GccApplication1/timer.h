/*
 * timer.h
 *
 * Created: 08-May-21 6:03:27 PM
 *  Author: fedea
 */ 


#ifndef TIMER_H_
#define TIMER_H_

//Funciones para inicializar el timer
//y actualizar los segundos
void TIMER_Init(unsigned char,unsigned char,unsigned char); //pide horas,minutos y segundos para inicializar
void TIMER_Update();

//Funcion para conseguir la hora
//En formato hh:mm:ss
unsigned char* TIMER_GetHora(unsigned char*);

//Funciones para modificar hora minutos o segundos
//En caso de ingresar valores inválidos no se realizará ningún cambio
void TIMER_ModificarHora(unsigned char);
void TIMER_ModificarMinutos(unsigned char);
void TIMER_ModificarSegundos(unsigned char);



#endif /* TIMER_H_ */