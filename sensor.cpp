#include <Arduino.h>
#include "config.h"

int sensorState[SENSOR_COUNT];

void setupSensors() {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    pinMode(sensorPins[i], INPUT);
  }
}

void readSensors() {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    sensorState[i] = digitalRead(sensorPins[i]);
  }
}

float calculatePosition() {
  int sum = 0, count = 0;
  for (int i = 0; i < SENSOR_COUNT; i++) {
    if (sensorState[i] == 0) { // 0 = line detected
      sum += (i - 2);
      count++;
    }
  }
  return (count > 0) ? ((float)sum / count) : 0;
}

bool lineDetected() {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    if (sensorState[i] == 0) return true;
  }
  return false;
}
