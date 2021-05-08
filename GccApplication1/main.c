/*
 * GccApplication1.c
 *
 * Created: 06-May-21 12:19:19 AM
 * Author : fedea
 */ 

#include "main.h"
#include "lcd.h"
#include "KeypadScanLib.h"
#include "MEF.h"
#inclued "seos.h"

int main(void)
{
	//Variables
	// uint8_t a=0;
	//Inicializar periféricos
	
	KEYPAD_Init();
	LCDinit();
	
	//Inicializar MEF
	
	MEF_Init(10);
	
	
	//Configurar Arq time-triggered
	//Recibe ms para el timer
	SEOS_Init(100);
	
	//Habilitar interrupciones
	sei();
	
	
	
    while (1) 
    {	
		/*
		if(KEYPAD_Scan(&a)){
			LCDGotoXY(8,0);
			LCDsendChar(a);
		}
		*/
		
		MEF_Update();
		_delay_ms(100);
		//realizar  tareas
    }
}

