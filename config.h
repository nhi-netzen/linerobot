#ifndef CONFIG_H
#define CONFIG_H

// ====== Motor Pins (TB6612FNG) ======
#define AIN1 12
#define AIN2 13
#define BIN1 9
#define BIN2 8
#define PWMA 11
#define PWMB 6
#define STBY 10

// ====== Motor Direction Offsets ======
const int offsetA = 1;
const int offsetB = 1;

// ====== Line Sensor Array (5 sensors) ======
#define SENSOR_COUNT 5
const int sensorPins[SENSOR_COUNT] = {2, 3, 4, 5, 7};
extern int sensorState[SENSOR_COUNT];

// ====== PID Constants ======
extern float Kp;
extern float Ki;
extern float Kd;

// ====== Line Lost Timeout ======
const unsigned long LINE_LOST_TIMEOUT = 750;

// ====== Motor Speeds ======
const int MIN_SPEED = 20;
const int MAX_SPEED = 255;
extern int baseSpeed;

#endif
