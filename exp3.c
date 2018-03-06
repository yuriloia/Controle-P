#include <ADuc842.h>
#include <stdio.h>

unsigned int i=0;
unsigned int bufferad = 0;
unsigned int temp = 0;
unsigned int duty = 0;
float Duty_Cycle = 0;
float valor_ad_conv = 0;
float valor_ad = 0;
int controladorP =20;

void configura_serial(void);
void configura_PWM (void);

void main (void){
	PLLCON = 0x80;
	ADCCON1 = 0x80;
	configura_serial();
	configura_PWM();

	while(1){
		for(i=0; i<16; i++){
			ADCCON2 = (ADCCON2 & 0xF0) | 0x04;
			SCONV = 1;
			while (SCONV);
			bufferad = ADCDATAL | ((ADCDATAH & 0x0F) << 8);
			temp = temp + bufferad;
		}
		valor_ad  =(temp/16);
		if(valor_ad>3300 && controladorP>=20)
			controladorP -=20;
		if (valor_ad<2800 && controladorP<=4075)
			controladorP +=20;
		if(valor_ad>=2800 && valor_ad<=3300)
			controladorP = controladorP;
		temp =0;
		valor_ad_conv = valor_ad*0.00122070;
		duty = (controladorP * 65535)/4095;		 //valor_ad varia de 0~~4095 
		PWM0H = duty >>8;
		PWM0L = (duty & 0x00FF);

		Duty_Cycle = (duty*100.0)/65535.0;
		printf("Valor Analogico: %f  Duty Cycle: %d\n", valor_ad, controladorP);
	}
}


void configura_serial(void){
	T3CON = 0x85;
	T3FD = 0x2d;
	SCON = 0x52;
}
void configura_PWM(void){
	PWMCON = 0x93;
	PWM1H = 0xFF;
	PWM1L = 0xFF;
}