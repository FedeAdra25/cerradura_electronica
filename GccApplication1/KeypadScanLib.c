/*
 * KeypadScanLib.c
 *
 * Created: 19-Apr-21 4:50:53 PM
 *  Author: fedea
 */

#include "KeypadScanLib.h"
#include <inttypes.h>
#include <avr/io.h>

static const uint8_t teclas[4][4]= {
	{'7','8','9','A'},
	{'4','5','6','B'},
	{'1','2','3','C'},
	{'*','0','#','D'},
};

/*
Retorna 0 si no se presionó ninguna tecla
Retorna 1 si se presionó una tecla (y lo hace en la variable *key)
supone que la memoria está alocada para *key
*/
uint8_t KEYPAD_Scan(uint8_t *key)
{
	const uint8_t aux[4]={0x7F,0xBF,0xDF,0xEF}; //{F1,F2,F3,F4} -> aux[i] implica 0 en Fila i
	uint8_t i;
	PORTD|=0xF0;
	for(i=0;i<4;i++){
		PORTD&=aux[i]; //va poniendo un cero en cada fila
		if(~PIND & 1<<PORTD0){
			//1 en Pin D0
			*key=teclas[i][0];
			return 1;
		}
		if(~PIND & 1<<PORTD1){
			//1 en Pin D1
			*key=teclas[i][1];
			return 1;
		}
		if(~PIND & 1<<PORTD2){
			//1 en Pin D2
			*key=teclas[i][2];
			return 1;
		}
		if(~PIND & 1<<PORTD3){
			//1 en Pin D3
			*key=teclas[i][3];
			return 1;
		}
	}
	return 0;
}

/********************************************************
FUNCION PARA ESCANEAR UN TECLADO MATRICIAL Y DEVOLVER LA
TECLA PRESIONADA UNA SOLA VEZ. TIENE DOBLE VERIFICACION Y
MEMORIZA LA ULTIMA TECLA PRESIONADA
DEVUELVE:
0 -> NO HAY NUEVA TECLA PRESIONADA
1 -> HAY NUEVA TECLA PRESIONADA Y ES *pkey
********************************************************/

uint8_t KEYPAD_Update (uint8_t *pkey)
{
	static uint8_t Old_key;
	static uint8_t Key, Last_valid_key=0xFF; // no hay tecla presionada
	if(!KEYPAD_Scan(&Key)) {
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