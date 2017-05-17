#ifndef sensor_H_
#define sensor_H_

int[3] proximity_s;
int[3] floor_s;
int[2] buttons_s;
float[3] battery_s;

void setup_sensors (void);
void read (void);

#endif