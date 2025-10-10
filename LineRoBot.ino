#include <Arduino.h>
#include <SparkFun_TB6612.h>
#include "config.h"
#include "PID.h"
#include "motor_control.h"
#include "sensor.h"

void setup() {
  Serial.begin(9600);
  setupSensors();
  setupMotors();
  pidInit();
  Serial.println("PID Line Follower Started");
  delay(1000);
}

void loop() {
  readSensors();
  if (!lineDetected()) {
    stopMotorsIfLineLost();
    return;
  }

  float position = calculatePosition();
  float correction = pidCompute(position);

  applyMotorSpeeds(correction);

  printDebugInfo(position, correction);
  delay(10);
}
