/*
 * MEF.c
 *
 * Created: 7/5/2021 15:32:08
 *  Author: jeron
 */ 
#include "MEF.h"
#include "KeypadScanLib.h"
//Variables globales privadas
static  uint8_t state_time = 0;
static MEF_state system_state;
static uint8_t claveAct[4] = {'0','8','5','2'};
static uint8_t key;
static uint8_t claveIng[4];
static uint8_t firstTimeIdle= 1;
static uint8_t firstTimeClaveInc= 1;
static uint8_t firstTimeClaveCor= 1;
static uint8_t posClaveIng= 0;
static uint32_t ticksPerSecond; //Se inicializa en el init

//Prototipos de funciones privadas 
static uint8_t ClaveCorrecta(void);
static uint8_t Clave_IncTime(void);



void MEF_update (void)
{
	//Cuento el numero de interrupciones, para calcular el tiempo en cada estado
	state_time++;
	switch (system_state)
	{
		case IDLE :
			if (KEYPAD_Update(&key))
			{
				switch key
				{
					case 'A':
					break;
					case 'B':
					break;
					case 'C':
					break;
					case 'D':
					break;
					default:
						posClaveIng=0; 
						changeING_CLAVE();
					break;
				}
				
				state_time=0;
			}
			else {
					if (firstTimeIdle)
					{
						LCDclr();
						//Imprimo hora
						LCDGotoXY(4,1);
						LCDstring("CERRADO", 7);
						firstTimeIdle = 0;
					}
					//if actualizo hora imprimir HH:MM:SS
				 }
		break;
		case ING_CLAVE :
			if (KEYPAD_Update(&key) && (posClaveIng < 4))
			{
				Out_IngClave();
			}
			else if (posClaveIng >= 4)
				 {
					if(ClaveCorrecta()) ChangeABIERTO();
					else ChangeCLAVE_INC();
				 }		
		break;
		case CLAVE_INC :
			if(firstTimeClaveInc)
			{
				firstTimeClaveInc=0;
				OutClaveInc();
			}
			else if (Clave_IncTime())
				{
					ChangeIDLE();
				}
		break;
		case ABIERTO:
		break;
		case M_CLAVE :
		break;
		case M_CLAVE_E :
		break;
		case M_CLAVE_F :
		break;
		case M_CLAVE_N:
		break;
		case M_HORA :
		break;
	}
	
	/***************************************************************
		Funcion que sirve para transicionar del estado IDLE, al 
		estado ING_CLAVE, guarda el caracter ingresado por el usuario,
		cambia el estado actual e imprimer el * en el LCD
	***************************************************************/
	void changeING_CLAVE (void)
	{
		system_state= ING_CLAVE;
		claveIng[posClaveIng]= key;
		LCDclr();
		LCDGotoXY(4,1);
		LCDsendChar('*');
		posClaveIng++;
	}
	/***************************************************************
		Funcion que sirve para guardar el dato ingresado por el usuario
		y mostrar el * en el LCD de salida del sistema
	***************************************************************/
	void Out_IngClave (void)
	{
		claveIng[posClaveIng]= key;
		//LCDGotoXY(4+posClaveIng, 1); Por si el cursor no se actualiza solo
		LCDsendChar('*');
		posClaveIng++;
	}
	
	/***************************************************************
		Funcion que sirve para que sirve para comparar la clave
		ingresada por el usuario con la contraseņa actual del 
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
	
	void ChangeABIERTO(void)
	{
		state_time=0;
		system_state= ABIERTO;
	}
	
	void ChangeCLAVE_INC(void)
	{
		state_time=0;
		firstTimeClaveInc=1;
		system_state= CLAVE_INC;
	}
	
	/***************************************************************
	Funcion que sirve para mostrar la salida de la MEF al estado
	CLAVE_INC, el mismo consiste en mostrar en el segundo renglon del
	LCD "DENEGADO" por 2 segundos
	***************************************************************/
	void OutClaveInc(void)
	{
		LCDclr();
		LCDGotoXY(4,1);
		LCDstring("DENEGADO", 8);
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
	
	void ChangeIDLE(void)
	{
		firstTimeIdle= 1;
		system_state= IDLE;
	}