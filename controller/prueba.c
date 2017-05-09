#include <string.h>
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "motor.h"

int main (void)
{
	int motorPins[4] = {17, 18, 22, 23};
	wiringPiSetupGpio();
	/*setupMotors(motorPins, 4);
	setSpeed(0, 100); setSpeed(1, 0); delay(2000);
	setSpeed(0, 0); setSpeed(1, 100); delay(2000);*/
	pinMode(22, OUTPUT);
	pinMode(23, OUTPUT);
	softPwmCreate(22, 0, 100);
	softPwmCreate(23, 0, 100);
	softPwmWrite(22, 50);
	softPwmWrite(23, 0); delay(2000);
	softPwmWrite(22, 0);
	softPwmWrite(23, 50); delay(2000);

	printf("fin\n");
	softPwmWrite(22, 0);
	softPwmWrite(23, 0); delay(500);
	//stopMotors();
	return 0 ;
}
