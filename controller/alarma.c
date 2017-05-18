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

#define VEL_INI 50									// Velocidad base inicial
#define VEL_MED 25									// Velocidad base bateria media
#define VEL_LOW 0									// Velocidad base bateria baja

#define BATT_MED 3.5									// Nivel a partir del cual la bateria se considera media
#define BATT_LOW 3.4									// Nivel a partir del cual la bateria se considera baja

typedef enum {RUNNING, SNOOZE, MED_BATT, LOW_BATT, STOP} estados;			// Distintos estados en los que puede estar el robot

struct sigaction sa,osa;

int main (void)
{
	estados state;
	int base_vel = VEL_INI;

   	memset(&sa, 0, sizeof(sa));							// Expandimos la funcionalidad al llegar una señal de interrupcion
    	sa.sa_handler = &bypass_sigint;							// Para asegurarnos de apagar los motores y el buzzer si esto ocurre
    	sigaction(SIGINT, &sa,&osa);

	wiringPiSetupGpio();								// Setup de la interfaz GPIO

	state = RUNNING;								// Estado inicial

	setup_actuators();								// Setup de los actuadores
	setup_sensors();								// Setup de los sensores

	int timeAplaz;

	while(1)
	{
		read();									// Leo los sensores

		if (state == RUNNING)							// Estamos en el estado en el que nos movemos
		{
			int vel_izq;
			int vel_der;

			if(proximity_s[1] || floor_s[1]){
				vel_der = base_vel;
				vel_izq = -base_vel;
			} else {

				vel_der = (1 - floor_s[0]) * (1 - proximity_s[0]) * base_vel;
				vel_izq = (1 - floor_s[2]) * (1 - proximity_s[2]) * base_vel;
			}

			set_speed(0, vel_izq);						// Ponemos una velocidad en la rueda derecha
			set_speed(1, vel_der);						// Ponemos una velocidad en la rueda izquierda
			buzzer_on();							// Encendemos el buzzer

			if (buttons_s[0] == HIGH)						// Si se pulsa el boton de apago de la alarma
			{
				state = STOP;						// Pasamos al estado de STOP
			}
			else if (buttons_s[1] == HIGH)					// Si se pulsa el boton de aplazar la alarma
			{
				state = SNOOZE;						// Pasamos al estado de SNOOZE
				timeAplaz = millis();					// Iniciamos el contador del tiempo
				buzzer_off();						// Apagamos el buzzer
				stop_motors();						// Apagamos los motores
			}
			else if ( fmin(fmin(battery_s[0], battery_s[1]), battery_s[2]) < BATT_MED && base_vel == VEL_INI)
			{
				state = MED_BATT;
			}
			else if ( fmin(fmin(battery_s[0], battery_s[1]), battery_s[2]) < BATT_LOW && base_vel == VEL_MED)
			{
				state = LOW_BATT;
			}
		}
		else if (state == SNOOZE)						// Estamos en el estado de aplazar la alarma
		{
			if (buttons_s[0] == HIGH)						// Si se pulsa el boton de apago de la alarma pasamos al estado de FIN
			{
				state = STOP;
			}
			else if (millis() - timeAplaz >= 600000)			// Si pasan 10 minutos desde el aplazamiento, vuelve a sonar la alarma
			{
				state = RUNNING;
			}
		}
		else if (state == MED_BATT)						// Estamos en el estado de bateria media, a partir de ahora se moverá a velocidad reducida
		{
			base_vel = VEL_MED;
		}
		else if (state == LOW_BATT)						// Estamos en el estado de bateria baja, a partir de ahora dejara de moverse
		{
			base_vel = VEL_LOW;
		}
		else if (state == STOP)							// Estamos en el estado de finalizar la alarma
		{
			buzzer_off();							// Paramos el buzzer
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
