#ifndef MOTOR_H_
#define MOTOR_H_

int* motorPins;
int size;
void setupMotors(int* pins, int nPins);
void setSpeed(int motor, int speed);
void stopMotors();

#endif