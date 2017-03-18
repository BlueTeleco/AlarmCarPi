#include "motor.h"
#include <wiringPi.h>
#include <softPwm.h>

int* motorsPins;
int size;

void setupMotors(int* pins, int nPins){
	int i;
	size = nPins;
	motorPins = pins;
	for ( i = 0; i < nPins; i++ ) {
		pinMode(*(pins + i), PWM_OUTPUT);
		softPwmCreate(*(pins + i), 0, 100);
	}
}

void setSpeed(int motor, int speed){
	if(speed >= 0){
		softPwmWrite(*(motorPins + 2 * motor), 0); softPwmWrite(*(motorPins + 2 * motor + 1), speed);
	} else {
		softPwmWrite(*(motorPins + 2 * motor), speed); softPwmWrite(*(motorPins + 2 * motor + 1), 0);
	}
}

void stopMotors()
{
	int i;
	for ( i = 0; i < size; i++ ) {
		softPwmWrite(*(motorPins + i), 0);
	}
	delay(100);
}