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
#include "seos.h"
#include "timer.h"

int main(void)
{
	//Variables
	// uint8_t a=0;
	//Inicializar periféricos
	
	KEYPAD_Init();
	LCDinit();
	
	//Inicializar MEF
	
	MEF_Init(10);
	
	//Inicializar Timer (para el reloj)
	TIMER_Init();
	
	
	//Configurar Arq time-triggered
	//La libreria interrumpe cada 20ms
	SEOS_Init();
	
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
		SEOS_Dispatch_Tasks();
		
		//realizar  tareas
    }
}

