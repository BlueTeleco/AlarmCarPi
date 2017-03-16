#include <string.h>
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "motor.h"

int main (void)
{
	int motorPins[4] = {17, 18, 22, 23};
	wiringPiSetupGpio();
	setupMotors(motorPins, 4);
	setSpeed(0, 100); setSpeed(1, 0); delay(2000);
	setSpeed(0, 0); setSpeed(1, 100); delay(2000);

	printf("fin\n");
	setSpeed(0, 0);
	setSpeed(1, 0);
	delay(1000);
	return 0 ;
}
