#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

void setupMotors();
void applyMotorSpeeds(float correction);
void stopMotorsIfLineLost();
void printDebugInfo(float position, float correction);

#endif
