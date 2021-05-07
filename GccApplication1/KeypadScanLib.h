/*
 * KeypadScanLib.h
 *
 * Created: 19-Apr-21 4:52:13 PM
 *  Author: fedea
 */


#ifndef KEYPADSCANLIB_H_
#define KEYPADSCANLIB_H_
#include <inttypes.h>
#include <avr/io.h>


/************************************************
Librería para leer teclado matricial
	
	row/col x -> mcu pin i
	
	COL1 -> KEYPAD_PORT_Pin0
	COL2 -> KEYPAD_PORT_Pin1
	COL3 -> KEYPAD_PORT_Pin2
	COL4 -> KEYPAD_PORT_Pin3
	ROW1 -> KEYPAD_PORT_Pin7
	ROW2 -> KEYPAD_PORT_Pin6
	ROW3 -> KEYPAD_PORT_Pin5
	ROW4 -> KEYPAD_PORT_Pin4
	

***************************************************/
//Configuracion del puerto que usa el teclado
//Cambiar todos los define si se quiere cambiar el puerto
//*************************
	#define KEYPAD_PORT PORTD
	#define KEYPAD_DDR DDRD
	#define KEYPAD_PIN PIND
	
	#define KEYPAD_PORT0 PORTD0
	#define KEYPAD_PORT1 PORTD1
	#define KEYPAD_PORT2 PORTD2
	#define KEYPAD_PORT3 PORTD3
	#define KEYPAD_PORT4 PORTD4
	#define KEYPAD_PORT5 PORTD5
	#define KEYPAD_PORT6 PORTD6
	#define KEYPAD_PORT7 PORTD7
//*************************



void KEYPAD_Init();

uint8_t KEYPAD_Scan (uint8_t *pkey); //funcion para escanear con doble verificacion y memoria


#endif /* KEYPADSCANLIB_H_ */