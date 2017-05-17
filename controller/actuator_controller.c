													// Nuestros drivers
#include "actuator_controller.h"
													// Biblioteca WirinPi para la interfaz GPIO
#include <wiringPi.h>
#include <softPwm.h>

#define PIN_BUZZER 27											// Numero de pin del buzzer

#define MOTORS 2											// Motores que tiene el robot
#define NUM_PINS_MOT 2											// Numero de pines que usa cada motor
const int motor_pins[MOTORS*NUM_PINS_MOT] = {17, 18, 22, 23};						// Pines de nuestros motores

													// Setup de los actuadores, usando la libreria WiringPi
void setup_actuators (){
	int i;
	for ( i = 0; i < MOTORS*NUM_PINS_MOT; i++ ) {
		pinMode(*(motor_pins + i), OUTPUT);
		softPwmCreate(*(pins + i), 0, 100);
	}
	
	pinMode(PIN_BUZZER, OUTPUT);
}
													// Funcion que da una velociad 'speed' al motor 'motor'
void set_speed (int motor, int speed){
	if(speed >= 0){
		softPwmWrite(*(motor_pins + 2 * motor), 0); softPwmWrite(*(motor_pins + 2 * motor + 1), speed);
	} else {
		softPwmWrite(*(motor_pins + 2 * motor), -speed); softPwmWrite(*(motor_pins + 2 * motor + 1), 0);
	}
}
													// Funcion que para todos los motores
void stop_motors ()
{
	int i;
	for ( i = 0; i < size; i++ ) {
		softPwmWrite(*(motorsPins + i), 0);
	}
	delay(100);
}
													// Función para activar el buzzer
void buzzer_on () 
{
	digitalWrite(PIN_BUZZER, HIGH);
}
													// Función para apagar el buzzer
void buzzer_off () 
{
	digitalWrite(PIN_BUZZER, LOW);
	delay(100);
}
