#include <Arduino.h>
#include <SparkFun_TB6612.h>
#include "config.h"
#include "PID.h"
#include "motor_control.h"
#include "sensor.h"
#include <QTRSensors.h>
void setup() {
  Serial.begin(9600);
  
  // Set the pins the for the (fake) qtr sensor
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);
  // qtr.setEmitterPin(2);
  
  Serial.println("QTR-8A calibration starting...");
  Serial.println("Move sensor across the line(s) for 10 seconds");
  
  // Turn on the built in LED to signal beginning calibration
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  delay(500);
  
  // Starts the calibration
  for (uint16_t i = 0; i < 400; i++) {
    qtr.calibrate();
  }
  
  // Turn off the built in LED to signal calibration completion
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.println("Calibration Complete!");
  
  // Print out the calibration results
  Serial.print("Min values: ");
  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  Serial.print("Max values: ");
  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  
  Serial.println("PID Line Follower Started");
  // Time to manually reposition the robot
  delay(2000);  
  
  lineDetectedTime = millis();
}

void loop() {
  // Read values from the sensor (0-5000: left-right)
  uint16_t position = qtr.readLineBlack(sensorValues);
  
  // Check if any line is detected
  bool lineDetected = false;
  for (uint8_t i = 0; i < SensorCount; i++) {
    if (sensorValues[i] > 200) {
      lineDetected = true;
      break;
    }
  }
