#include <Arduino.h>
#include "config.h"

// PID Variables
float Kp = 70.0;
float Ki = 0.0;
float Kd = 10.0;

float error = 0;
float lastError = 0;
float integral = 0;
float derivative = 0;

void pidInit() {
  error = lastError = integral = derivative = 0;
}

float pidCompute(float position) {
  error = 0 - position;
  integral += error;
  derivative = error - lastError;
  float correction = (Kp * error) + (Ki * integral) + (Kd * derivative);
  lastError = error;
  return correction;
}
