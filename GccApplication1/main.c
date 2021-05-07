/*
 * GccApplication1.c
 *
 * Created: 06-May-21 12:19:19 AM
 * Author : fedea
 */ 

#include "main.h"
#include "lcd.h"
#include "KeypadScanLib.h"

int main(void)
{
	//Variables
	uint8_t a=0;
	//Inicializar periféricos
	
	KEYPAD_Init();
	LCDinit();
	
	//Inicializar MEF
	
	//mefinit
	
	
	//Configurar Arq time-triggered
	//sEOS init
	
	//Habilitar interrupciones
	sei();
	
	
	
    while (1) 
    {
		if(KEYPAD_Scan(&a)){
			LCDGotoXY(8,0);
			LCDsendChar(a);
		}
		_delay_ms(100);
		//realizar  tareas
    }
}

