// Nuestros drivers
#include "sensors.h"
// Biblioteca WirinPi para la interfaz GPIO
#include <wiringPi.h>

int* sensorsPins;										// Puntero al array de los pines de los motores
int* sensorsAngles;
int size;												// Numero de pines en el array

// Setup de los sensores, usando la libreria WiringPi
void setupProximitySensors(int* pins, int* angles, int nPins){
	int i;
	size = nPins;
	sensorsPins = pins;
	sensorsAngles = angles;
	for ( i = 0; i < nPins; i++ ) {
		pinMode(*(pins + i), INPUT);
		pullUpDnControl(*(pins + i), PUD_DOWN);
	}
}

// Funcion que da una velociad 'speed' al motor 'motor'
void readProximitySensors(){
	int i;
	float angle = 0;
	for ( i = 0; i < size; i++ ) {
		angle += !digitalRead(*(pins + i)) * sensorsAngles(*(pins + i));
	}
	return angle / size;
}
