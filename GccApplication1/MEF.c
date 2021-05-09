/*
 * MEF.c
 *
 * Created: 7/5/2021 15:32:08
 *  Author: jeron
 */ 
#include "MEF.h"
#include "KeypadScanLib.h"
#include "lcd.h"
#include "timer.h"
#include <stdlib.h>

// Constantes
const uint8_t lengthClaveAct=4;

//Variables globales privadas
static  uint8_t state_time = 0;
static MEF_state system_state;
//Se usa para la maquina de subestados al cambiar la hora
static MEF_H_substate hora_substate;
static uint8_t claveAct[4] = {'0','8','5','2'};
static uint8_t key;
static uint8_t claveIng[4];
static uint8_t posClaveIng= 0;
static uint32_t ticksPerSecond; //Se inicializa en el init
static uint8_t actHora = 0; 
static unsigned char* hora;
static uint8_t ingresoDig;
static uint8_t horaIng;
static uint8_t teclaPresionada=0;


//Prototipos de funciones privadas 
static uint8_t ClaveCorrecta(void);
static uint8_t Clave_IncTime(void);
static uint8_t AbiertoTime(void);

//Funciones para modelar transiciones de estados
static void changeING_CLAVE (void);
static void ChangeABIERTO(void);
static void ChangeCLAVE_INC(void);
static void ChangeIDLE(void);
static void ChangeM_CLAVE(void);
static void ChangeM_CLAVE_N (void);
static void ChangeM_CLAVE_F(void);
static void ChangeM_CLAVE_E(void);
static void changeM_HORA_H(void);
static void changeM_HORA_M(void);
static void changeM_HORA_S(void);


//Funciones para modelar cambios de salida de la MEF
static void Out_IngClave (void);
static void OutClaveInc(void);
static void OutAbierto(void);
static void OutIdle(void);
static void OutMClave(void);
static void OutMClaveN(void);
static void OutMClaveE(void);
static void OutMClaveF(void);
static void OutMHoraH(void);
static void OutMHoraM(void);
static void OutMHoraS(void);



void MEF_Init(uint32_t tps){
	ticksPerSecond=tps;
	system_state=IDLE;
}


void MEF_Update (void)
{
	//Cuento el numero de interrupciones, para calcular el tiempo en cada estado
	state_time++;
	actHora++;
	teclaPresionada= KEYPAD_Scan(&key);
	switch (system_state)
	{
		case IDLE:
			if (teclaPresionada)
			{
				switch (key)
				{
					case 'A':
						changeM_HORA_H();
					break;
					case 'B':
						changeM_HORA_M();
					break;
					case 'C':
						changeM_HORA_S();
					break;
					case 'D':
						ChangeM_CLAVE();
					break;
					default: 
						changeING_CLAVE();
					break;
				}
				
			}
			else {
					OutIdle();
					//if actualizo hora imprimir HH:MM:SS
				 }
		break;
		case ING_CLAVE :
			if (teclaPresionada && (posClaveIng < 4))
			{
				if(key != 'A' && key != 'B' && key != 'C' && key != 'D' && key != '#' && key != '*')
				{
					Out_IngClave();
				}
			}
			if (posClaveIng == 4)
			 {
				if(ClaveCorrecta()) ChangeABIERTO();
				else ChangeCLAVE_INC();
			 }		
		break;
		case CLAVE_INC :
			if(state_time == 1)
			{
				OutClaveInc();
			}
			else if (Clave_IncTime())
				{
					ChangeIDLE();
				}
		break;
		case ABIERTO:
			if(state_time == 1)
			{
				OutAbierto();
			}
			else if (AbiertoTime())
			{
				ChangeIDLE();
			}
		break;
		case M_CLAVE :
			if (state_time == 1 )
			{
				OutMClave();
			}
			if(teclaPresionada)
			{
				if (key!='A' && key!='B' && key!='C' && key!='*' && key!='D' && key!='#' && posClaveIng < 4)
				{
					Out_IngClave();
				}
				if(posClaveIng==4){
					if (ClaveCorrecta()) ChangeM_CLAVE_N();
					else ChangeCLAVE_INC();
				}			
			}
		break;
		case M_CLAVE_N:
			if(state_time==1)
			{
				 OutMClaveN();
			}
			if( teclaPresionada)
			{
				if (key == 'D')
				{
					if (posClaveIng == 4)
					{
						uint8_t i;
						for (i=0; i<4;i++)
						{
							claveAct[i]= claveIng[i];
						}
						ChangeM_CLAVE_F();
					}
					else ChangeM_CLAVE_E();
				}
				else if (key == '#')
					{
						ChangeIDLE();
					}
				else if (key!='A' && key!='B' && key!='C' && key!='*')
					{
						if (posClaveIng < 4) Out_IngClave();
						else posClaveIng++;
					}
			}
		break;
		case M_CLAVE_F :
			if (state_time==1)
			{
				OutMClaveF();
			}
			//Reutilizo AbiertoTime, ya que tiene que permanecer 3 seg en este estado al igual que lo que pasaba con abierto
			if(AbiertoTime())
			{
				ChangeIDLE();
			}
		break;
		case M_CLAVE_E :
			if(state_time==1)
			{
				OutMClaveE();
			}
			//Reutilizo AbiertoTime, ya que tiene que permanecer 3 seg en este estado al igual que lo que pasaba con abierto
			if(AbiertoTime())
			{
				ChangeIDLE();
			}
		break;
		case M_HORA:
			switch(hora_substate)
			{
				case M_HORA_H:
					if(state_time==1) OutMHoraH();
					if(teclaPresionada){
						if (key != 'A' && key != 'B' && key != 'C' && key != 'D' && key != '*' && key != '#')
						{
							if (ingresoDig== 0)
							{
								ingresoDig++;
								hora[0]=key; //para ir cambiandola en el display
								horaIng= (key-'0') * 10;							
								
							} 
							else if(ingresoDig==1){
								ingresoDig++;
								hora[1]=key; //para ir cambiandola en el display
								horaIng+= (key-'0');
							}
							OutMHoraH();
						
						}
						else if( key == 'A')
						{
							TIMER_ModificarHora(horaIng);
							ChangeIDLE();							
						}
						else if(key == '#')
						{
							ChangeIDLE();
						}
						}
				break;
				case M_HORA_M:
					if(state_time==1) OutMHoraM();
					if(teclaPresionada){						
						if (key != 'A' && key != 'B' && key != 'C' && key != 'D' && key != '*' && key != '#')
						{
							if (ingresoDig==0)
							{
								ingresoDig++;
								hora[3]=key;
								horaIng= (key-'0') *10;
							}
							else if(ingresoDig==1){
								ingresoDig++;
								hora[4]=key;
								horaIng+= (key-'0');
							}
							OutMHoraM();
						}
						else if( key == 'B')
							{
								TIMER_ModificarMinutos(horaIng);
								ChangeIDLE();
						
							}
						else if(key == '#')
						{
							ChangeIDLE();
						}
					}
				break;
				case M_HORA_S: 
					if(state_time==1) OutMHoraS();
					if(teclaPresionada){
						
						if (key != 'A' && key != 'B' && key != 'C' && key != 'D' && key != '*' && key != '#')
						{
							if (ingresoDig == 0)
							{
								ingresoDig++;
								hora[6]=key;
								horaIng= (key-'0') *10;
							}
							else if(ingresoDig==1){
								ingresoDig++;
								hora[7]=key;
								horaIng+= (key-'0');
							}
							OutMHoraS();
						}
						else if( key == 'C')
						{
							TIMER_ModificarSegundos(horaIng);
							ChangeIDLE();
						
						}
						else if(key == '#')
						{
							ChangeIDLE();
						}
					}
				break;
			}
			
		break;
	}
	
}
	
	/***************************************************************
		Funcion que sirve mostrar la salida del estado IDLE, en este
		caso se muestra la hora y el mensaje CERRADO
	***************************************************************/
	static void OutIdle(void)
	{
		if(state_time==1)
		{
			LCDclr();
			LCDGotoXY(4,0);
			hora = TIMER_GetHora(hora);
			LCDstring(hora, 8);
			LCDGotoXY(4,1);
			LCDstring((uint8_t*)"CERRADO",(uint8_t) 7);
		}
		if (actHora == ticksPerSecond)
		{
			hora = TIMER_GetHora(hora);
			LCDGotoXY(4,0);
			LCDstring(hora, 8);
			actHora=0;
		}		
	}
	/***************************************************************
		Funcion que sirve para transicionar del estado IDLE, al 
		estado ING_CLAVE, guarda el caracter ingresado por el usuario,
		cambia el estado actual e imprimer el * en el LCD
	***************************************************************/
	static void changeING_CLAVE (void)
	{
		state_time=0;
		system_state= ING_CLAVE;
		claveIng[posClaveIng]= key;
		posClaveIng++;
		LCDclr();
		LCDGotoXY(6,1);
		LCDsendChar('*');
	}
	/***************************************************************
		Funcion que sirve para guardar el dato ingresado por el usuario
		y mostrar el * en el LCD de salida del sistema
	***************************************************************/
	static void Out_IngClave (void)
	{
		claveIng[posClaveIng]= key;
		//LCDGotoXY(6+posClaveIng, 1);
		LCDsendChar('*');
		posClaveIng++;
	}
	
	/***************************************************************
		Funcion que sirve para que sirve para comparar la clave
		ingresada por el usuario con la contrase�a actual del 
		sistema
	***************************************************************/
	
	static uint8_t ClaveCorrecta(void)
	{
			uint8_t  i=0;
			for(i=0; i<4; i++)
			{
				if (claveIng[i] != claveAct[i]) return 0;
			}
			return 1;
	}
	
	static void ChangeABIERTO(void)
	{
		state_time=0;
		system_state= ABIERTO;
	}
	
	static void ChangeCLAVE_INC(void)
	{
		state_time=0;
		system_state= CLAVE_INC;
	}
	
	/***************************************************************
	Funcion que sirve para mostrar la salida de la MEF al estado
	CLAVE_INC, el mismo consiste en mostrar en el segundo renglon del
	LCD "DENEGADO" por 2 segundos
	***************************************************************/
	static void OutClaveInc(void)
	{
		LCDclr();
		LCDGotoXY(4,1);
		LCDstring((uint8_t*)"DENEGADO", 8);
	}
	
	/***************************************************************
	Funcion que determina si se cumplio el tiempo en el estado 
	CLAVE_INC, en este caso chequea si el tiempo en dicho estado (en
	ticks), es igual al tiempo tambien en ticks (por eso se multiplica
	 el valor 2 correspondiente a los seg que se debe esta en dicho 
	 estado por la cantidad de ticks por segundo) que se debe estar 
	 en dicho estado
	***************************************************************/
	static uint8_t Clave_IncTime(void)
	{
		if(state_time < 2*ticksPerSecond) return 0;
		else return 1;
	}
	
	static void ChangeIDLE(void)
	{
		state_time=0;
		posClaveIng=0;
		actHora=0;
		system_state= IDLE;
		LCDcursorOFF();
	}
	
	
	/***************************************************************
		Funcion que sirve mostrar la salida del estado ABIERTO, en este
		caso se muestra el mensaje ABIERTO
	***************************************************************/
	static void OutAbierto(void)
	{
		LCDclr();
		LCDGotoXY(4,1);
		LCDstring((uint8_t*)"ABIERTO",(uint8_t) 7);
	}
	
	/***************************************************************
	Funcion que determina si se cumplio el tiempo en el estado 
	ABIERTO, en este caso chequea si el tiempo en dicho estado (en
	ticks), es igual al tiempo tambien en ticks (por eso se multiplica
	 el valor 3 correspondiente a los seg que se debe esta en dicho 
	 estado por la cantidad de ticks por segundo) que se debe estar 
	 en dicho estado
	***************************************************************/
	static uint8_t AbiertoTime(void)
	{
		if(state_time < 3*ticksPerSecond) return 0;
		else return 1;
	}
	
	static void ChangeM_CLAVE(void)
	{
		state_time=0;
		posClaveIng=0;
		system_state= M_CLAVE;
	}
	
	/***************************************************************
		Funcion que sirve mostrar la salida del estado M_CLAVE, en este
		caso se muestra el mensaje "clave actual:", y a medida que se
		presionan las teclas *
	***************************************************************/
	static void OutMClave(void)
	{
			LCDclr();
			LCDstring((uint8_t*)" Clave Actual:", 13);
			LCDGotoXY(6,1);
	}
	
	static void ChangeM_CLAVE_N(void)
	{
		state_time=0;
		posClaveIng=0;
		system_state= M_CLAVE_N;
	}
	
	/***************************************************************
		Funcion que sirve mostrar la salida del estado M_CLAVE_N, en este
		caso se muestra el mensaje "clave nueva:", y a medida que se
		presionan las teclas *
	***************************************************************/
	static void OutMClaveN(void)
	{
			LCDclr();
			LCDstring((uint8_t*)" Clave nueva:", 13);
			LCDGotoXY(6,1);
	}
	
	static void ChangeM_CLAVE_F(void)
	{
		state_time=0;
		system_state= M_CLAVE_F;
	}
	
	static void ChangeM_CLAVE_E(void)
	{
		state_time=0;
		system_state= M_CLAVE_E;
	}
	
	
	/***************************************************************
		Funcion que sirve mostrar la salida del estado M_CLAVE_F, en este
		caso se muestra el mensaje "clave nueva:"
	***************************************************************/
	static void OutMClaveF(void)
	{
		LCDclr();
		LCDGotoXY(2,0);
		LCDstring((uint8_t*)" Fin inicio",11);
		LCDGotoXY(2,1);
		LCDstring((uint8_t*)" Nueva clave", 12);
	}
	
	
	/***************************************************************
		Funcion que sirve mostrar la salida del estado M_CLAVE_E, en este
		caso se muestra el mensaje "ERROR
									Clave Invalida"
	***************************************************************/
	static void OutMClaveE(void)
	{
		LCDclr();
		LCDGotoXY(4,0);
		LCDstring((uint8_t*)"ERROR",5);
		LCDGotoXY(1,1);
		LCDstring((uint8_t*)"Clave Invalida", 14);
	}
	
	static void changeM_HORA_H(void)
	{
		system_state= M_HORA;
		ingresoDig=0;
		state_time=0;
		horaIng=0;
		hora_substate=M_HORA_H;
	}
	
	static void changeM_HORA_M(void)
	{
		system_state= M_HORA;
		ingresoDig=0;
		state_time=0;
		horaIng=0;
		hora_substate=M_HORA_M;
	}
	
	static void changeM_HORA_S(void)
	{
		system_state= M_HORA;
		ingresoDig=0;
		state_time=0;
		horaIng=0;
		hora_substate=M_HORA_S;
	}
	/***************************************************************
		Funcion que sirve mostrar la salida del estado M_HORA al querer
		modificar las horas, en este caso se mostra la hora actual con 
		el cursor en el digito a cambiar
	***************************************************************/
	static void OutMHoraH(void)
	{
		if (state_time== 1)
		{
			LCDclr();
			LCDGotoXY(4,0);
			LCDstring(hora, 8);
			LCDGotoXY(4,0);
			LCDcursorOnBlink();
		}
		else{
			LCDGotoXY(4,0);
			LCDstring(hora, 8);
			LCDGotoXY(4+ingresoDig,0);
			LCDcursorOnBlink();			
		}
	}
	
	/***************************************************************
		Funcion que sirve mostrar la salida del estado M_HORA al querer
		modificar los minutos, en este caso se mostra la hora actual con 
		el cursor en el digito a cambiar
	***************************************************************/
	static void OutMHoraM(void)
	{
		if (state_time == 1)
		{
			LCDclr();
			LCDGotoXY(4,0);
			LCDstring(hora, 8);
			LCDGotoXY(7,0);
			LCDcursorOnBlink();
		}
		else{
			LCDGotoXY(4,0);
			LCDstring(hora, 8);
			LCDGotoXY(7+ingresoDig,0);
			LCDcursorOnBlink();
		}
		
	}
	
	/***************************************************************
		Funcion que sirve mostrar la salida del estado M_HORA al querer
		modificar los segundos, en este caso se mostra la hora actual con 
		el cursor en el digito a cambiar
	***************************************************************/
	static void OutMHoraS(void)
	{
		if (state_time== 1)
		{
			LCDclr();
			LCDGotoXY(4,0);
			LCDstring(hora, 8);
			LCDGotoXY(10,0);
			LCDcursorOnBlink();
		}
		else{
			LCDGotoXY(4,0);
			LCDstring(hora, 8);
			LCDGotoXY(10+ingresoDig,0);
			LCDcursorOnBlink();
		}
		
	}