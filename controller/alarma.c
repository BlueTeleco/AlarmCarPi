											// Librerias estandar
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <signal.h>
											// Librerias para la interfaz con GPIO
#include <wiringPi.h>
											// Drivers propios
#include "alarma.h"
#include "actuator_controller.h"
#include "sensor_controller.h"


typedef enum {CORRIENDO, APLAZAR, FIN} estados;						// Distintos estados en los que puede estar el robot

struct sigaction sa,osa;

int main (void)
{
	estados state;

   	memset(&sa, 0, sizeof(sa));							// Expandimos la funcionalidad al llegar una señal de interrupcion
    	sa.sa_handler = &bypass_sigint;							// Para asegurarnos de apagar los motores y el buzzer si esto ocurre
    	sigaction(SIGINT, &sa,&osa);

	wiringPiSetupGpio();								// Setup de la interfaz GPIO

	state = CORRIENDO;								// Estado inicial

	setup_actuators();								// Setup de los actuadores
	setup_sensors();								// Setup de los sensores
	
	int timeAplaz;
	int* proximity = proximity();
	int* floor = floor();
	int* buttons = buttons(); 

	while(1)									
	{
		read();									// Leo los sensores
		
		if (state == CORRIENDO)							// Estamos en el estado en el que nos movemos
		{
			int vel_izq;
			int vel_der;
			
			if(proximity[1]){
				vel_der = 50;
				vel_izq = -50;
			} else {

				vel_der = (1 - proximity[0]) * 50;
				vel_izq = (1 - proximity[2]) * 50;
			}

			set_speed(0, vel_izq);						// Ponemos una velocidad en la rueda derecha
			set_speed(1, vel_der);						// Ponemos una velocidad en la rueda izquierda
			buzzer_on();							// Encendemos el buzzer

			if (buttons[0] == HIGH)						// Si se pulsa el boton de apago de la alarma
			{
				state = FIN;						// Pasamos al estado de FIN
			}
			else if (buttons[1] == HIGH)					// Si se pulsa el boton de aplazar la alarma
			{
				state = APLAZAR;					// Pasamos al estado de APLAZAR
				timeAplaz = millis();					// Iniciamos el contador del tiempo
				buzzer_off();						// Apagamos el buzzer
				stop_motors();						// Apagamos los motores
			}
		}
		else if (state == APLAZAR)						// Estamos en el estado de aplazar la alarma
		{
			if (buttons[0] == HIGH)						// Si se pulsa el boton de apago de la alarma pasamos al estado de FIN
			{
				state = FIN;
			}
			else if (millis() - timeAplaz >= 600000)			// Si pasan 10 minutos desde el aplazamiento, vuelve a sonar la alarma
			{
				state = CORRIENDO;
			}
		}
		else if (state == FIN)							// Estamos en el estado de finalizar la alarma
		{
			buzzer_off();					// Paramos el buzzer
			stopMotors();							// Paramos los motores
			break;								// Terminamos
		}
	}

	return 0;
}

void bypass_sigint(int sig_no)								// En caso de llegada de la señal de interrupcion
{
	digitalWrite(PIN_BUZZER, LOW);
 	stopMotors();

	sigaction(SIGINT,&osa,NULL);
	kill(0,SIGINT);
}
