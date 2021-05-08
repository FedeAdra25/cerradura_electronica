/*
 * MEF.c
 *
 * Created: 7/5/2021 15:32:08
 *  Author: jeron
 */ 
#include "MEF.h"
#include "KeypadScanLib.h"

// Constantes
const uint8_t lengthClaveAct=4;

//Variables globales privadas
static  uint8_t state_time = 0;
static MEF_state system_state;
static uint8_t claveAct[4] = {'0','8','5','2'};
static uint8_t key;
static uint8_t claveIng[4];
static uint8_t posClaveIng= 0;
static uint32_t ticksPerSecond; //Se inicializa en el init

//Prototipos de funciones privadas 
static uint8_t ClaveCorrecta(void);
static uint8_t Clave_IncTime(void);
static uint8_t AbiertoTime(void);



void MEF_Update (void)
{
	//Cuento el numero de interrupciones, para calcular el tiempo en cada estado
	state_time++;
	switch (system_state)
	{
		case IDLE :
			if (KEYPAD_Scan(&key))
			{
				switch (key)
				{
					case 'A':
					break;
					case 'B':
					break;
					case 'C':
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
			if (KEYPAD_Scan(&key) && (posClaveIng < 4))
			{
				Out_IngClave();
			}
			if (posClaveIng >= 4)
			 {
				if(ClaveCorrecta()) ChangeABIERTO();
				else ChangeCLAVE_INC();
			 }		
		break;
		case CLAVE_INC :
			if(state_time == 0)
			{
				OutClaveInc();
			}
			else if (Clave_IncTime())
				{
					ChangeIDLE();
				}
		break;
		case ABIERTO:
			if(state_time == 0)
			{
				OutAbierto();
			}
			else if (AbiertoTime())
			{
				ChangeIDLE();
			}
		break;
		case M_CLAVE :
			if (state_time==0 )
			{
				OutMClave();
			}
			if(KEYPAD_Scan(&key))
			{
				if (key!='A' && key!='B' && key!='C' && key!='*' && posClaveIng < 4)
				{
					Out_IngClave();
				}
			}
			if (ClaveCorrecta()) ChangeM_CLAVE_N();
			else ChangeCLAVE_INC();
		break;
		case M_CLAVE_N:
			if(state_time == 0)
			{
				 OutMClaveN();
			}
			if( KEYPAD_Scan(&key))
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
						else posClaveIng ++;
					}
			}
		break;
		case M_CLAVE_F :
			if (state_time==0)
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
			if(state_time==0)
			{
				OutMClaveE();
			}
			//Reutilizo AbiertoTime, ya que tiene que permanecer 3 seg en este estado al igual que lo que pasaba con abierto
			if(AbiertoTime())
			{
				ChangeIDLE();
			}
		break;
		case M_HORA :
		break;
	}
}
	
	/***************************************************************
		Funcion que sirve mostrar la salida del estado IDLE, en este
		caso se muestra la hora y el mensaje CERRADO
	***************************************************************/
	void OutIdle(void)
	{
		if(state_time==0)
		{
			LCDclr();
			LCDGotoXY(4,1);
			LCDstring("CERRADO", 7);
		}
		//LCDGotoXY(,0);
		//Imprimo hora
		
	}
	/***************************************************************
		Funcion que sirve para transicionar del estado IDLE, al 
		estado ING_CLAVE, guarda el caracter ingresado por el usuario,
		cambia el estado actual e imprimer el * en el LCD
	***************************************************************/
	void changeING_CLAVE (void)
	{
		state_time=0;
		system_state= ING_CLAVE;
		claveIng[posClaveIng]= key;
		posClaveIng++;
	}
	/***************************************************************
		Funcion que sirve para guardar el dato ingresado por el usuario
		y mostrar el * en el LCD de salida del sistema
	***************************************************************/
	void Out_IngClave (void)
	{
		if (state_time == 0 && system_state == ING_CLAVE)
		{
			LCDclr();
			LCDGotoXY(4,1);
			LCDsendChar('*');
		}
		claveIng[posClaveIng]= key;
		LCDGotoXY(4+posClaveIng, 1);
		LCDsendChar('*');
		posClaveIng++;
	}
	
	/***************************************************************
		Funcion que sirve para que sirve para comparar la clave
		ingresada por el usuario con la contraseña actual del 
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
		state_time=0;
		posClaveIng=0;
		system_state= IDLE;
	}
	
	
	/***************************************************************
		Funcion que sirve mostrar la salida del estado ABIERTO, en este
		caso se muestra el mensaje ABIERTO
	***************************************************************/
	void OutAbierto(void)
	{
		LCDclr();
		LCDGotoXY(4,1);
		LCDstring("ABIERTO", 7);
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
	
	void ChangeM_CLAVE(void)
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
	void OutMClave(void)
	{
			LCDclr();
			LCDstring("Clave Actual:", 13);
			LCDGotoXY(4,1);
	}
	
	void ChangeM_CLAVE_N(void)
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
	void OutMClaveN(void)
	{
			LCDclr();
			LCDstring("Clave nueva:", 12);
			LCDGotoXY(4,1);
	}
	
	void ChangeM_CLAVE_F(void)
	{
		state_time=0;
		system_state= M_CLAVE_F;
	}
	
	void ChangeM_CLAVE_E(void)
	{
		state_time=0;
		system_state= M_CLAVE_E;
	}
	
	
	/***************************************************************
		Funcion que sirve mostrar la salida del estado M_CLAVE_F, en este
		caso se muestra el mensaje "clave nueva:"
	***************************************************************/
	void OutMClaveF(void)
	{
		LCDclr();
		LCDGotoXY(4,0);
		LCDstring("fin inicio",10);
		LCDGotoXY(4,1);
		LCDstring("nueva clave", 11);
	}
	
	
	/***************************************************************
		Funcion que sirve mostrar la salida del estado M_CLAVE_E, en este
		caso se muestra el mensaje "ERROR
									Clave Invalida"
	***************************************************************/
	void OutMClaveE(void)
	{
		LCDclr();
		LCDGotoXY(4,0);
		LCDstring("ERROR",5);
		LCDGotoXY(1,1);
		LCDstring("Clave Invalida", 14);
	}