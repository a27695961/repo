#include <reg51.h>
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
//void delay(int t);

sbit DHT11=P2^1;		/* Connect DHT11 output Pin to P2.1 Pin */
int I_RH,D_RH,I_Temp,D_Temp,CheckSum; 
code char err[] = "output error";
code char hum[] = "Humidity = ";
code char tempp[] = "Temp = ";


void timer_delay20ms()		/* Timer0 delay function */
{
	TMOD = 0x21;
	TH0 = 0xB8;		/* Load higher 8-bit in TH0 */
	TL0 = 0x0C;		/* Load lower 8-bit in TL0 */
	TR0 = 1;		/* Start timer0 */
	while(!TF0);	/* Wait until timer0 flag set */
	TR0 = 0;		/* Stop timer0 */
	TF0 = 0;		/* Clear timer0 flag */
}

void timer_delay30us()		/* Timer0 delay function */
{
	TMOD = 0x21;		/* Timer0 mode1 (16-bit timer mode) */
	TH0 = 0xFF;		/* Load higher 8-bit in TH0 */
	TL0 = 0xE8;		/* Load lower 8-bit in TL0 */
	TR0 = 1;		/* Start timer0 */
	while(TF0 == 0);	/* Wait until timer0 flag set */
	TR0 = 0;		/* Stop timer0 */
	TF0 = 0;		/* Clear timer0 flag */
}
void Request()			/* Microcontroller send request */
{	
	DHT11 = 1;
	timer_delay20ms();
	DHT11 = 0;		/* set to low pin */
	timer_delay20ms();	/* wait for 20ms */
	DHT11 = 1;		/* set to high pin */
	while(DHT11==1);
	while(DHT11==0);
	while(DHT11==1);
}
int Receive_data()		/* Receive data */
{
	int q,c=0;	
	for (q=0; q<8; q++)
	{
		while(!DHT11);/* check received bit 0 or 1 */
		timer_delay30us();
		
		c <<=1;
		if(DHT11 == 1){
			c |= (0x01);/* Then its logic HIGH */
			while(DHT11);
		}
		
		
		//while(DHT11==1);
	}
	return c;
}

void serial_output_err(void)
{
 //unsigned char	buffer;
	char *buf;
    int n;
	char itoc[40];
	TH1 = 0Xfd;          //11.0592, baud 9600bps
	TL1 = 0xfd;
	TR1 = 1;   
	
	for(buf = err ; *buf != '\0' ;){
		SBUF = *buf++;
		while(TI == 0);
		TI = 0;
	}


	n = sprintf(itoc,"%d.%d, ", I_RH,D_RH);
	for(buf = itoc ; *buf != '\0' ;){
		SBUF = *buf++;
		while(TI == 0);
		TI = 0;
	}
	n = sprintf(itoc,"%d.%d, ", I_Temp,D_Temp);
	for(buf = itoc ; *buf != '\0' ;){
		SBUF = *buf++;
		while(TI == 0);
		TI = 0;
	}


	n = sprintf(itoc,"%d\n", CheckSum);
	for(buf = itoc ; *buf != '\0' ;){
		SBUF = *buf++;
		while(TI == 0);
		TI = 0;
	}
	//TR1 = 0;
}

void serial_output()
{
 //unsigned char	buffer;
	char itoc[40];
	float humi,tempera;
	char* buf;
	int i = 0;
	int n = 0;

	TH1 = 0Xfd;          //11.0592, baud 9600bps
	TL1 = 0xfd;
	TR1 = 1;   
	
	humi = (float)( (I_RH*256 )+ D_RH)/10;
	tempera = (float)( (I_Temp*256  )+D_Temp)/10;
	
	for(buf = hum ; *buf != '\0' ;){
		SBUF = *buf++;
		while(TI == 0);
		TI = 0;
	}

	/*n = sprintf(itoc,"%d.%d %%\n", I_RH,D_RH);
	for(buf = itoc ; *buf != '\0' ;){
		SBUF = *buf++;
		while(TI == 0);
		TI = 0;
	}*/
	n = sprintf(itoc,"%.1f %% \t", humi);
	for(buf = itoc ; *buf != '\0' ;){
		SBUF = *buf++;
		while(TI == 0);
		TI = 0;
	}

	for(buf = tempp ; *buf != '\0' ;){
		SBUF = *buf++;
		while(TI == 0);
		TI = 0;
	}

	/*n = sprintf(itoc,"%d.%d C\n", I_Temp,D_Temp);
	for(buf = itoc ; *buf != '\0' ;){
		SBUF = *buf++;
		while(TI == 0);
		TI = 0;
	}*/
	n = sprintf(itoc,"%.1f C \n", tempera);
	for(buf = itoc ; *buf != '\0' ;){
		SBUF = *buf++;
		while(TI == 0);
		TI = 0;
	}
}

void main()
{
	while(1){
		timer_delay20ms();
		timer_delay20ms();
		timer_delay20ms();
		timer_delay20ms();
		timer_delay20ms();
		SM0 = 0;
		SM1 = 1;
		TMOD = 0x20;  

		Request();	/* send start pulse */
		
		I_RH=Receive_data();	/* store first eight bit in I_RH */
		D_RH=Receive_data();	/* store next eight bit in D_RH */
		I_Temp=Receive_data();	/* store next eight bit in I_Temp */
		D_Temp=Receive_data();	/* store next eight bit in D_Temp */
		CheckSum=Receive_data();/* store next eight bit in CheckSum */
		
		if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum)
		{
			serial_output_err();
			//serial_output(I_RH,D_RH,I_Temp,D_Temp,CheckSum);
		}
		
		else
		{
			serial_output();
		}			
		timer_delay20ms();
		TR1 = 0;
		timer_delay20ms();timer_delay20ms();timer_delay20ms();timer_delay20ms();timer_delay20ms();timer_delay20ms();timer_delay20ms();timer_delay20ms();timer_delay20ms();timer_delay20ms();timer_delay20ms();timer_delay20ms();timer_delay20ms();timer_delay20ms();timer_delay20ms();timer_delay20ms();timer_delay20ms();timer_delay20ms();
	}
}