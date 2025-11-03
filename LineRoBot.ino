#include <Arduino.h>
#include <SparkFun_TB6612.h>
#include "config.h"
#include "PID.h"
#include "motor_control.h"
#include "sensor.h"
#include <QTRSensors.h>
void setup() {
  Serial.begin(9600);
  
  // Set sensor pins as input
  for (int i = 0; i < 5; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  
  Serial.println("PID Line Follower Started");
  delay(1000);  // Give time to position robot
  
  lineDetectedTime = millis();  // Initialize timer
}

void loop() {
  // Step 1: Read sensors
  for (int i = 0; i < 5; i++) {
    sensorState[i] = digitalRead(sensorPins[i]);
  }
  
  // Step 2: Calculate position
  float position = calculatePosition();
  
  // Check if line is detected (any sensor reads 0)
  bool lineDetected = false;
  for (int i = 0; i < 5; i++) {
    if (sensorState[i] == 0) {  // If any sensor detects line
      lineDetected = true;
      break;
    }
  }
  
  // Update line detected timer
  if (lineDetected) {
    lineDetectedTime = millis();  // Reset timer when line is detected
  }
  
  // Check how long line has been lost
  unsigned long timeSinceLineDetected = millis() - lineDetectedTime;
  
  // If line lost for longer than timeout, stop motors
  if (timeSinceLineDetected > LINE_LOST_TIMEOUT) {
    motor1.brake();
    motor2.brake();
    Serial.print("LINE LOST FOR ");
    Serial.print(timeSinceLineDetected);
    Serial.println("ms - STOPPING");
    
    // Reset PID variables
    integral = 0;
    lastError = 0;
    
    delay(100);
    return;  // Skip rest of loop
  }
  
  // Step 3: Calculate error (target position is 0)
  error = 0 - position;
  
  // Step 4: Calculate PID terms
  float P = Kp * error;
  
  integral = integral + error;
  float I = Ki * integral;
  
  derivative = error - lastError;
  float D = Kd * derivative;
  
  // Step 5: Calculate correction
  correction = P + I + D;
  
  // Step 6: Apply correction to motor speeds
  leftSpeed = baseSpeed + correction;
  rightSpeed = baseSpeed - correction;
  
  // Step 7: Constrain speeds to valid range (MIN_SPEED to MAX_SPEED)
  leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
  rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
  
  // Step 8: Drive motors
  // Positive speed = forward, negative = backward
  motor1.drive(leftSpeed);
  motor2.drive(rightSpeed);
  
  // Step 9: Update last error for next iteration
  lastError = error;
  
  // Debug output (optional - comment out for better performance)
  printDebugInfo(position);
  
  delay(10);  // Small delay for loop stability
}

// Function to calculate weighted position from sensor readings
float calculatePosition() {
  int sum = 0;
  int count = 0;
  
  // Assign weights: -2, -1, 0, 1, 2 for sensors 0-4
  for (int i = 0; i < 5; i++) {
    if (sensorState[i] == 0) {  // Changed: 0 = line detected
      sum += (i - 2);  // Weight from -2 to +2
      count++;
    }
  }
  
  // Calculate average position
  if (count > 0) {
    return (float)sum / count;
  } else {
    // No line detected - return 0 (will be handled in main loop)
    return 0;
  }
}

// Debug function to print sensor and PID info
void printDebugInfo(float position) {
  Serial.print("Sensors: ");
  for (int i = 0; i < 5; i++) {
    Serial.print(sensorState[i]);
    Serial.print(" ");
  }
  
  Serial.print("| Pos: ");
  Serial.print(position, 2);
  
  Serial.print(" | Error: ");
  Serial.print(error, 2);
  
  Serial.print(" | Corr: ");
  Serial.print(correction, 2);
  
  Serial.print(" | L: ");
  Serial.print(leftSpeed);
  
  Serial.print(" R: ");
  Serial.println(rightSpeed);
}
