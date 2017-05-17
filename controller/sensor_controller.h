#ifndef sensor_H_
#define sensor_H_

extern int proximity_s[3];
extern int floor_s[3];
extern int buttons_s[2];
extern float battery_s[3];

void setup_sensors (void);
void read (void);

#endif
