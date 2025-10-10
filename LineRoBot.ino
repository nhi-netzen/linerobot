#include <SparkFun_TB6612.h>

// Set pins for the TB6612FNG
#define AIN1 12
#define BIN1 9
#define AIN2 13
#define BIN2 8
#define PWMA 11
#define PWMB 6
#define STBY 10

// Direction switch
const int offsetA = 1;
const int offsetB = 1;

// Initialize motors
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

// Set the pins for the sensor array
const int sensorPins[5] = {2, 3, 4, 5, 7};
int sensorState[5];

// PID Constants - Tune these values
float Kp = 70.0;   // Proportional gain
float Ki = 0.0;    // Integral gain (start at 0)
float Kd = 10.0;   // Derivative gain

// PID Variables
float error = 0;
float lastError = 0;
float integral = 0;
float derivative = 0;
float correction = 0;

// Line loss detection timer
unsigned long lineDetectedTime = 0;  // Last time line was detected
const unsigned long LINE_LOST_TIMEOUT = 750;  // Time in ms before stopping (adjust as needed)

// Motor speeds
const int MIN_SPEED = 20;   // Minimum speed threshold for motors
const int MAX_SPEED = 255;  // Maximum speed
int baseSpeed = 100;  // Base speed for both motors (gives room for corrections)
int leftSpeed = 0;
int rightSpeed = 0;

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
