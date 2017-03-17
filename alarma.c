#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <signal.h>
#include "motor.h"

#define MOTORES 2															// Motores que tiene el robot
#define NUM_PINES_MOT 2														// Numero de pines que usa cada motor

#define PIN_FIN 15															// Numero de pin del boton de fin
#define PIN_APLAZA 14														// Numero de pin del boton de aplazar
#define PIN_BUZZER 3														// Numero de pin del buzzer

#define PIN_PROX_I 2														// Numero de pin del sensor de proximidad izquierdo
#define PIN_PROX_C 3														// Numero de pin del sensor de proximidad centro
#define PIN_PROX_D 4														// Numero de pin del sensor de proximidad derecho


typedef enum {CORRIENDO, APLAZAR, FIN} estados;								// Distintos estados en los que puede estar el robot

struct sigaction osa;

void bypass_sigint(int sig_no);

int main (void)
{
	estados state;
	struct sigaction sa,osa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &bypass_sigint;
    sigaction(SIGINT, &sa,&osa);

	wiringPiSetupGpio();													// Setup de la interfaz GPIO

	pinMode(PIN_FIN , INPUT);												// Boton que finaliza la alarma
	pinMode(PIN_APLAZA, INPUT);												// Boton que aplaza la alarma
	pinMode(PIN_BUZZER, OUTPUT);											// Buzzer

	pinMode(PIN_PROX_I, INPUT);												// Sensor de proximidad izquierdo
	pinMode(PIN_PROX_C, INPUT);												// Sensor de proximidad centro
	pinMode(PIN_PROX_D, INPUT);												// Sensor de proximidad derecho

	pullUpDnControl(PIN_FIN, PUD_DOWN);
	pullUpDnControl(PIN_APLAZA, PUD_DOWN);
	pullUpDnControl(PIN_PROX_I, PUD_DOWN);
	pullUpDnControl(PIN_PROX_D, PUD_DOWN);
	pullUpDnControl(PIN_PROX_C, PUD_DOWN);


	state = CORRIENDO;														// Estado inicial

	int timeAplaz;

	int pines_motores[MOTORES*NUM_PINES_MOT] = {17, 18, 22, 23};			// Pines de nuestros motores
	setupMotors(pines_motores, MOTORES*NUM_PINES_MOT);						// Setup de los motores

	while(1)																// Mientras que no se haya pulsado el boton para finalizar la alarma (no estamos en el estado FIN)
	{
		if (state == CORRIENDO)												// Estamos en el estado en el que nos movemos
		{
			int prox_i = digitalRead(PIN_PROX_I);
			int prox_d = digitalRead(PIN_PROX_I);
			int prox_c = digitalRead(PIN_PROX_I);

			int vel_der = (1 - prox_i) * pow(-1, prox_c) * 50;
			int vel_izq = (1 - prox_d) * pow(1, prox_c) * 50;

			setSpeed(0, vel_izq);											// Ponemos una velocidad en la rueda derecha
			setSpeed(1, vel_der);											// Ponemos una velocidad en la rueda izquierda
			digitalWrite(PIN_BUZZER, HIGH);									// Encendemos el buzzer

			if (digitalRead(PIN_FIN) == HIGH)								// Si se pulsa el boton de apago de la alarma
			{
				state = FIN;												// Pasamos al estado de FIN
			}
			else if (digitalRead(PIN_APLAZA) == HIGH)						// Si se pulsa el boton de aplazar la alarma
			{
				state = APLAZAR;											// Pasamos al estado de APLAZAR
				timeAplaz = millis();										// Iniciamos el contador del tiempo
				digitalWrite(PIN_BUZZER, LOW);								// Apagamos el buzzer
				stopMotors();												// Apagamos los motores
			}
		}
		else if (state == APLAZAR)											// Estamos en el estado de aplazar la alarma
		{
			if (digitalRead(PIN_FIN) == HIGH)								// Si se pulsa el boton de apago de la alarma pasamos al estado de FIN
			{
				state = FIN;
			}
			else if (millis() - timeAplaz >= 600000)						// Si pasan 10 minutos desde el aplazamiento, vuelve a sonar la alarma
			{
				state = CORRIENDO;
			}
		}
		else if (state == FIN)												// Estamos en el estado de finalizar la alarma
		{
			digitalWrite(PIN_BUZZER, LOW);									// Paramos el buzzer
			stopMotors();													// Paramos los motores
			break;															// Terminamos
		}
	}

	return 0;
}

void bypass_sigint(int sig_no)
{
	digitalWrite(PIN_BUZZER, LOW);
 	stopMotors();
	
	sigaction(SIGINT,&osa,NULL);
    kill(0,SIGINT);
}