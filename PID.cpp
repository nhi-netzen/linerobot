#include <Arduino.h>
#include "config.h"

// PID values
float Kp = 0.1;
float Ki = 0.0;
float Kd = 0.5;

// PID variables
float error = 0;
float lastError = 0;
float integral = 0;
float derivative = 0;
float correction = 0;

error = 2500 - position;
  
  // Check if the error exceeds the correction threshold
  if (abs(error) > CORRECTION_THRESHOLD) {
    // Stop and rotate in place
    Serial.print("CORRECTION NEEDED (ಠಿ_ಠ) - Error: ");
    Serial.print(error);
    Serial.println(" - ROTATING");
    
    if (error > 0) {
      // Line is to the left - rotate left (counter-clockwise)
      motor1.drive(-ROTATION_SPEED);  // Left motor backward
      motor2.drive(ROTATION_SPEED);   // Right motor forward
    } else {
      // Line is to the right - rotate right (clockwise)
      motor1.drive(ROTATION_SPEED);   // Left motor forward
      motor2.drive(-ROTATION_SPEED);  // Right motor backward
    }
    
    // Don't update lastError during rotation
    delay(10);
    return;  // Skip normal PID control
  }
  
  // Normal PID control for small errors
  // Calculate the PID terms
  float P = Kp * error;
  
  integral = integral + error;
  float I = Ki * integral;
  
  derivative = error - lastError;
  float D = Kd * derivative;
  
  // Calculate correction
  correction = P + I + D;
  
