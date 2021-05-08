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