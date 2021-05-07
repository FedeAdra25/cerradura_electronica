/*
 * KeypadScanLib.c
 *
 * Created: 19-Apr-21 4:50:53 PM
 *  Author: fedea
 */

#include "KeypadScanLib.h"
#include <inttypes.h>
#include <avr/io.h>


//Definicion de variable global privada
static const uint8_t teclas[4][4]= {
	{'7','8','9','A'},
	{'4','5','6','B'},
	{'1','2','3','C'},
	{'*','0','#','D'},
};



//Prototipo para funcion privada 
static uint8_t getKeyPressed(uint8_t *key);


/*******************************************************
FUNCION PARA INICIALIZAR EL TECLADO MATRICIAL
********************************************************/
void KEYPAD_Init(){
	KEYPAD_DDR = 0xF0; //inicializo filas como salidas columnas como entradas
	KEYPAD_PORT = 0x0F; //activo pullup intenro
}

/********************************************************
FUNCION PARA ESCANEAR UN TECLADO MATRICIAL Y DEVOLVER LA
TECLA PRESIONADA UNA SOLA VEZ. TIENE DOBLE VERIFICACION Y
MEMORIZA LA ULTIMA TECLA PRESIONADA
DEVUELVE:
0 -> NO HAY NUEVA TECLA PRESIONADA
1 -> HAY NUEVA TECLA PRESIONADA Y ES *pkey
********************************************************/

uint8_t KEYPAD_Scan (uint8_t *pkey)
{
	static uint8_t Old_key;
	static uint8_t Key, Last_valid_key=0xFF; // no hay tecla presionada
	if(!getKeyPressed(&Key)) {
		Old_key=0xFF; // no hay tecla presionada
		Last_valid_key=0xFF;
		return 0;
	}
	if(Key==Old_key) { //2da verificación
		if(Key!=Last_valid_key){ //evita múltiple detección
			*pkey=Key;
			Last_valid_key = Key;
			return 1;
		}
	}
	Old_key=Key; //1era verificación
	return 0;
}


static uint8_t getKeyPressed(uint8_t *key)
{
	const uint8_t aux[4]={0x7F,0xBF,0xDF,0xEF}; //{F1,F2,F3,F4} -> aux[i] implica 0 en Fila i
	uint8_t i;
	KEYPAD_PORT|=0xF0;
	for(i=0;i<4;i++){
		KEYPAD_PORT&=aux[i]; //va poniendo un cero en cada fila
		if(~KEYPAD_PIN & 1<<KEYPAD_PORT0){
			//1 en Pin D0
			*key=teclas[i][0];
			return 1;
		}
		if(~KEYPAD_PIN & 1<<KEYPAD_PORT1){
			//1 en Pin D1
			*key=teclas[i][1];
			return 1;
		}
		if(~KEYPAD_PIN & 1<<KEYPAD_PORT2){
			//1 en Pin D2
			*key=teclas[i][2];
			return 1;
		}
		if(~KEYPAD_PIN & 1<<KEYPAD_PORT3){
			//1 en Pin D3
			*key=teclas[i][3];
			return 1;
		}
	}
	return 0;
}