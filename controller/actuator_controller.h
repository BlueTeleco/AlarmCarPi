#ifndef MOTOR_H_
#define MOTOR_H_

void setupMotors(int* pins, int nPins);
void setSpeed(int motor, int speed);
void stopMotors();

#endif