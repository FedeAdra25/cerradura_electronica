/*
 * main.h
 *
 * Created: 06-May-21 12:19:39 AM
 *  Author: fedea
 */ 


#ifndef MAIN_H_
#define MAIN_H_

//Archivo cabecera de microcontrolador
#include <avr/io.h>
//Interrupciones
#include <avr/interrupt.h>

//DEFINO FRECUENCIA DEL CLOCK DEL MCU (en Hz)
#define F_CPU 8000000UL //8Mhz
//delays segun frecuencia del clock
#include <util/delay.h>


//Tipos de datos enteros estandar
#include <stdint.h>
//Manejar caracteres
#include <stdlib.h>

#endif /* MAIN_H_ */

/* End of file */