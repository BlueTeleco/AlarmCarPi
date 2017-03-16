#include "motor.h"
#include <wiringPi.h>
#include <softPwm.h>

int* motorsPins;

void setupMotors(int* pins, int size){
	int i;
	motorPins = pins;
	for ( i = 0; i < size; i++ ) {
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