#include <string.h>
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "motor.h"

#define MOTORES 2							// Motores que tiene el robot
#define NUM_PINES_MOT 2							// Numero de pines que usa cada motor

#define PIN_FIN 1							// Numero de pin del boton de fin 
#define PIN_APLAZA 2							// Numero de pin del boton de aplazar
#define PIN_BUZZER 3							// Numero de pin del buzzer

enum estado {CORRIENDO, APLAZAR, FIN};					// Distintos estados en los que puede estar el robot

int main (void)
{
	wiringPiSetupGpio();						// Setup de la interfaz GPIO

	pinMode(PIN_FIN , INPUT);					// Boton que finaliza la alarma
	pinMode(PIN_APLAZA, INPUT);					// Boton que aplaza la alarma
	pinMode(PIN_BUZZER, OUTPUT);					// Buzzer

	digitalWrite(PIN_BUZZER, HIGH);					// Inicia el buzzer
	estados state = CORRIENDO;					// Estado inicial

	int timeAplaz;

	int pines_motores[MOTORES*NUM_PINES_MOT] = {};			// Pines de nuestros motores
	setupMotors(*pines_motores, MOTORES*NUM_PINES_MOT);		// Setup de los motores
		
	while(1)							// Mientras que no se haya pulsado el boton para finalizar la alarma (no estamos en el estado FIN)
	{
		if (state == CORRIENDO)					// Estamos en el estado en el que nos movemos
		{
			setSpeed(0, 50);				// Ponemos una velocidad en el motor 0
			setSpeed(1, 50);				// Ponemos una velocidad en el motor 1
			digitalWrite(PIN_BUZZER, HIGH);			// Encendemos el buzzer

			if (digitalRead(PIN_FIN) == HIGH)		// Si se pulsa el boton de apago de la alarma 
			{
				state = FIN;				// Pasamos al estado de FIN
			}	
			
			if (digitalRead(PIN_APLAZA) == HIGH)		// Si se pulsa el boton de aplazar la alarma 
			{
				state = APLAZAR;			// Pasamos al estado de APLAZAR		
				timeAplaz = millis();			// Iniciamos el contador del tiempo
				digitalWrite(PIN_BUZZER, LOW);		// Apagamos el buzzer
				stopMotors();				// Apagamos los motores
			}	
		}

		if (state == APLAZAR)					// Estamos en el estado de aplazar la alarma
		{
			if (millis() - timeAplaz >= 600000)		// Si pasan 10 minutos desde el aplazamiento, vuelve a sonar la alarma
			{
				state = CORRIENDO;
			} 

			if (digitalRead(PIN_FIN) == HIGH)		// Si se pulsa el boton de apago de la alarma pasamos al estado de FIN
			{
				state = FIN;			
			}	
		}

		if (state == FIN)					// Estamos en el estado de finalizar la alarma
		{
			digitalWrite(PIN_BUZZER, LOW);			// Paramos el buzzer 
			stopMotors();					// Paramos los motores
			break;						// Terminamos
		}
	}

	return 0;
}
