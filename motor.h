#ifndef MOTOR_H_
#define MOTOR_H_

int* motorPins;
void setupMotors(int* pins, int size);
void setSpeed(int motor, int speed);

#endif