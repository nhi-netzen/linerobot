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

// Varibles to switch the direction of the motors
const int offsetA = 1;
const int offsetB = 1;

// Initialize motors
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

// Set up the (fake) qtr sensor
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
// ====== PID Constants ======
extern float Kp;
extern float Ki;
extern float Kd;

// Timer for line loss detection
unsigned long lineDetectedTime = 0;
const unsigned long LINE_LOST_TIMEOUT = 777; // threshold (ms)

// Motor speeds
const int MIN_SPEED = 20;
const int MAX_SPEED = 255;
int baseSpeed = 140;
int leftSpeed = 0;
int rightSpeed = 0;

// Threshold for the robot to stop and correct itself
const int CORRECTION_THRESHOLD = 2000;
const int ROTATION_SPEED = 100;

#endif
