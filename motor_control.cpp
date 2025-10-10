#include <Arduino.h>
#include <SparkFun_TB6612.h>
#include "config.h"

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

int baseSpeed = 100;
unsigned long lineDetectedTime = 0;

void setupMotors() {
  lineDetectedTime = millis();
}

void applyMotorSpeeds(float correction) {
  int leftSpeed = baseSpeed + correction;
  int rightSpeed = baseSpeed - correction;

  leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
  rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);

  motor1.drive(leftSpeed);
  motor2.drive(rightSpeed);
}

void stopMotorsIfLineLost() {
  unsigned long elapsed = millis() - lineDetectedTime;
  if (elapsed > LINE_LOST_TIMEOUT) {
    motor1.brake();
    motor2.brake();
    Serial.print("LINE LOST ");
    Serial.print(elapsed);
    Serial.println(" ms → STOPPED");
  }
}

void printDebugInfo(float position, float correction) {
  Serial.print("Sensors: ");
  for (int i = 0; i < SENSOR_COUNT; i++) {
    Serial.print(sensorState[i]);
    Serial.print(" ");
  }
  Serial.print("| Pos: "); Serial.print(position, 2);
  Serial.print(" | Corr: "); Serial.print(correction, 2);
  Serial.print(" | Base: "); Serial.println(baseSpeed);
}
