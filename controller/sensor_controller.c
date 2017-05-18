											// Librerias estandar
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
											// Librerias para la interfaz con GPIO
#include <wiringPi.h>

#define PIN_FIN		15								// Numero de pin del boton de fin
#define PIN_APLAZA	14									// Numero de pin del boton de aplazar

#define PIN_PROX_I 2									// Numero de pin del sensor de proximidad izquierdo
#define PIN_PROX_C 3									// Numero de pin del sensor de proximidad centro
#define PIN_PROX_D 4									// Numero de pin del sensor de proximidad derecho

#define PIN_FLOOR_I 5									// Numero de pin del sensor de suelo izquierdo
#define PIN_FLOOR_C 6									// Numero de pin del sensor de suelo centro
#define PIN_FLOOR_D 7									// Numero de pin del sensor de suelo derecho

int proximity_s[3];									// Sensores de proximidad
int floor_s[3];										// Sensores de suelo
int buttons_s[2];									// Botones
float battery_s[3];
int fd;

void setup_sensors (void)
{
	pinMode(PIN_FIN , INPUT);							// Boton que finaliza la alarma
	pinMode(PIN_APLAZA, INPUT);							// Boton que aplaza la alarma

	pinMode(PIN_PROX_I, INPUT);							// Sensor de proximidad izquierdo
	pinMode(PIN_PROX_C, INPUT);							// Sensor de proximidad centro
	pinMode(PIN_PROX_D, INPUT);							// Sensor de proximidad derecho

	pinMode(PIN_FLOOR_I, INPUT);							// Sensor de suelo izquierdo
	pinMode(PIN_FLOOR_C, INPUT);							// Sensor de suelo centro
	pinMode(PIN_FLOOR_D, INPUT);							// Sensor de suelo derecho
	
	pullUpDnControl(PIN_FIN, PUD_DOWN);						// Ponemos una resistencia de pull down en nuestros pines
	pullUpDnControl(PIN_APLAZA, PUD_DOWN);
	pullUpDnControl(PIN_PROX_I, PUD_DOWN);
	pullUpDnControl(PIN_PROX_D, PUD_DOWN);
	pullUpDnControl(PIN_PROX_C, PUD_DOWN);
	pullUpDnControl(PIN_FLOOR_I, PUD_DOWN);
	pullUpDnControl(PIN_FLOOR_D, PUD_DOWN);
	pullUpDnControl(PIN_FLOOR_C, PUD_DOWN);

	fd = serialOpen("/dev/serial0", 9600);
}

// Funcion que lee los sensores
void read (void)
{
	int newChar;
	string number;
	int currentNumber;

	proximity_s[0] = !digitalRead(PIN_PROX_I);
	proximity_s[1] = !digitalRead(PIN_PROX_C);
	proximity_s[2] = !digitalRead(PIN_PROX_D);

	floor_s[0] = !digitalRead(PIN_FLOOR_I);
	floor_s[1] = !digitalRead(PIN_FLOOR_C);
	floor_s[2] = !digitalRead(PIN_FLOOR_D);

	buttons_s[0] = digitalRead(PIN_FIN);
	buttons_s[1] = digitalRead(PIN_APLAZA);

	if(serialDataAvail(fd) > 0) {
		numbers = 0;
		while(serialGetchar(fd) != '&') {}
		while((newChar = serialGetchar(fd)) != '\n') {
			switch (newChar) {
				case '\t':
				battery_s[currentNumber++] = atof(number);
				number = "";
				default:
				number.push_back(newChar);
			}
		}
		serialFlush(fd);
	}
}
