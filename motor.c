													// Nuestros drivers
#include "motor.h"
													// Biblioteca WirinPi para la interfaz GPIO
#include <wiringPi.h>
#include <softPwm.h>

int* motorsPins;									// Puntero al array de los pines de los motores
int size;											// Numero de pines en el array

// Setup de los motores, usando la libreria WiringPi
void setupMotors(int* pins, int nPins){
	int i;
	size = nPins;
	motorsPins = pins;
	for ( i = 0; i < nPins; i++ ) {
		pinMode(*(pins + i), OUTPUT);
		softPwmCreate(*(pins + i), 0, 100);
	}
}

// Funcion que da una velociad 'speed' al motor 'motor'
void setSpeed(int motor, int speed){
	if(speed >= 0){
		softPwmWrite(*(motorsPins + 2 * motor), 0); softPwmWrite(*(motorsPins + 2 * motor + 1), speed);
	} else {
		softPwmWrite(*(motorsPins + 2 * motor), -speed); softPwmWrite(*(motorsPins + 2 * motor + 1), 0);
	}
}

// Para todos los motores
void stopMotors()
{
	int i;
	for ( i = 0; i < size; i++ ) {
		softPwmWrite(*(motorsPins + i), 0);
	}
	delay(100);
}
