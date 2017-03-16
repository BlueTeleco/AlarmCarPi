#include <string.h>
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

enum estado {LENTO, MEDIO, RAPIDO, FIN};

int main (void)
{

	wiringPiSetupGpio();					// Setup de la interfaz GPIO

	pinMode(18, INPUT);					// Boton que finaliza la alarma

	pinMode(22, OUTPUT);					// Buzzer

	pinMode(23, PWM_OUTPUT);				// Salidas  del motor 1
	pinMode(24, PWM_OUTPUT);				// Con las que definiremos la velocidad y sentido
	
	pinMode(3, PWM_OUTPUT);					// Salidas  del motor 2
	pinMode(4, PWM_OUTPUT);					// Con las que definiremos la velocidad y sentido

	estados state = LENTO;					// Estado inicial
	int timeIni = millis();					// Tiempo inicial

	digitalWrite(17, HIGH);					// Inicia el motor 1
	digitalWrite(21, HIGH);					// Inicia el motor 2
	digitalWrite(22, HIGH);					// Inicia el buzzer
	softPwmCreate(18, 0, 100);				// Inicializa el motor 1
	softPwmCreate(22, 0, 100);				// Inicializa el motor 2

	while(state != FIN)					// Mientras que no se haya pulsado el boton para finalizar la alarma (no estamos en el estado FIN)
	{
		if (state == LENTO)				// Si estamos en el estado de movernos lentamente
		{
			if (millis() - timeIni >= 300000)	// Cuando pasan 5 minutos pasamos al siguiente estado, y volvemos a cambiar el valor inicial del contador
			{
				state = MEDIO;			
				timeIni = millis();
			}	

			if (digitalRead(18) == HIGH)		// Si se pulsa el boton de apago de la alarma pasamos al estado de fin
			{
				state = FIN;			
			}	

			softPwmWrite(23, 30);
			softPwmWrite(24, 30);
		}

		if (state == MEDIO)				// Si estamos en el estado de movernos a velocidad media
		{
			if (millis() - timeIni >= 600000)	// Cuando pasan 10 minutos m√s pasamos al siguiente estado
			{
				state = RAPIDO;			
			}	

			if (digitalRead(18) == HIGH)		// Si se pulsa el boton de apago de la alarma pasamos al estado de fin
			{
				state = FIN;			
			}	

			softPwmWrite(23, 60);
			softPwmWrite(24, 60);
		}

		if (state == RAPIDO)				// Si estamos en el estado de movernos rapidamente
		{
			if (digitalRead(18) == HIGH)		// Si se pulsa el boton de apago de la alarma pasamos al estado de fin
			{
				state = FIN;			
			}	

			softPwmWrite(23, 90);
			softPwmWrite(24, 90);
		}
	}

	softPwmWrite (23, 0);					// Paramos el motor 1
	softPwmWrite (24, 0);					// Paramos el motor 1
	digitalWrite(17, LOW);					// Paramos el motor 1
	digitalWrite(21, LOW);					// Paramos el motor 2 
	digitalWrite(22, LOW);					// Paramos el buzzer 
	return 0;
}
