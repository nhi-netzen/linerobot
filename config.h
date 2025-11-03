#ifndef CONFIG_H
#define CONFIG_H

// Set pins for the TB6612FNG
#define PWMA 11
#define AIN2 10
#define AIN1 9
#define STBY 8
#define BIN1 7
#define BIN2 6
#define PWMB 5

// Direction switch
const int offsetA = 1;
const int offsetB = 1;

// Initialize motors
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

// Set the pins for the sensor array
const int sensorPins[5] = {2, 3, 4, 12, 13};
int sensorState[5];

