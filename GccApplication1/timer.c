/*
 * timer.c
 *
 * Created: 08-May-21 6:03:43 PM
 *  Author: fedea
 */ 


static unsigned char seconds=0;
static unsigned char minutes=0;
static unsigned char hours=0;




void TIMER_Init(){
	
}

void TIMER_Update(){
	seconds++;
	if(seconds==60){
		seconds=0;
		minutes++;
	}
	if(minutes==60){
		minutes=0;
		hours++;
	}
	if(hours==24){
		hours=0;
	}
}


unsigned char* TIMER_GetHora(){
	unsigned char* hora = malloc(sizeof(char)*8);
	hora[0] = '0' + hours/10;
	hora[1] = '0' + hours%10;
	hora[2] = ':';
	hora[3] = '0' + minutes/10;
	hora[4] = '0' + minutes%10;
	hora[5] = ':';
	hora[6] = '0' + seconds/10;
	hora[7] = '0' + seconds%10;
	return hora;	
}

void TIMER_ModificarHora(uint8_t h){
	if(h<24 && h>=0){
		hours=h;
	}
}

void TIMER_ModificarMinutos(uint8_t m){
	if(m<60 && m>=0){
		minutes=m;
	}
	
}
void TIMER_ModificarSegundos(uint8_t s){
	if(s<60 && s>=0){
		seconds=s;
	}
}