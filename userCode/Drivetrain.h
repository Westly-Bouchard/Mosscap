//
// Created by Westly Bouchard on 4/2/26.
//

#ifndef INC_441SIM_DRIVETRAIN_H
#define INC_441SIM_DRIVETRAIN_H

#ifdef SIM
#include "../arduino/Motor.h"
#include "../arduino/Encoder.h"
#include <iostream>
#else
#include "Motor.h"
#include "Encoder.h"
#endif

#include "Controller.h"
#include "Trajectory.h"

using namespace std;

/*****************************************************************************
 *                                 Hardware                                  *
 *****************************************************************************/
#define CPR 900
#define TRACKWIDTH 0.209
#define WHEEL_RADIUS 0.04

#define FL_IN1 28
#define FL_IN2 26
#define FL_ENA 10
#define FL_A 3
#define FL_B 36

#define FR_IN1 24
#define FR_IN2 22
#define FR_ENA 9
#define FR_A 2
#define FR_B 38

#define BL_IN1 4
#define BL_IN2 6
#define BL_ENA 11
#define BL_A 19
#define BL_B 34

#define BR_IN1 7
#define BR_IN2 8
#define BR_ENA 12
#define BR_A 18
#define BR_B 40

Motor FL_M(FL_ENA, FL_IN1, FL_IN2);
Motor FR_M(FR_ENA, FR_IN1, FR_IN2);
Motor BL_M(BL_ENA, BL_IN1, BL_IN2);
Motor BR_M(BR_ENA, BR_IN1, BR_IN2);

Encoder FL_E(FL_A, FL_B);
Encoder FR_E(FR_A, FR_B);
Encoder BL_E(BL_A, BL_B);
Encoder BR_E(BR_A, BR_B);

/*****************************************************************************
 *                                  State                                    *
 *****************************************************************************/
double x = 0.0;
double y = 0.0;
double theta = 0.0;

double vX = 0.0;
double vY = 0.0;
double omega = 0.0;

void updatePosition() {
  static unsigned long lastTime = millis();

  unsigned long currentTime = millis();
  int dt = currentTime - lastTime;
  lastTime = currentTime;

  // Encoder counts from last update
  int FLCounts = FL_E.readAndReset();
  int BLCounts = BL_E.readAndReset();

  int FRCounts = FR_E.readAndReset();
  int BRCounts = BR_E.readAndReset();

  // Convert to distances
  double FLDist = (double)FLCounts / CPR * 2 * PI * WHEEL_RADIUS;
  double BLDist = (double)BLCounts / CPR * 2 * PI * WHEEL_RADIUS;

  double FRDist = (double)FRCounts / CPR * 2 * PI * WHEEL_RADIUS;
  double BRDist = (double)BRCounts / CPR * 2 * PI * WHEEL_RADIUS;

  // Average front and back distances b/c tank kinematics
  double dL = (FLDist + BLDist) / 2.0;
  double dR = (FRDist + BRDist) / 2.0;

  // Change in distance and angle
  double dS = 0.5 * (dL + dR);
  double dTheta = (dR - dL) / TRACKWIDTH * 180 / PI;

  double dY = dS * sin((theta + 0.5 * dTheta) * PI / 180);
  double dX = dS * cos((theta + 0.5 * dTheta) * PI / 180);

  // Convert to cartesian coordinates
  x += dX;
  y += dY;
  theta += dTheta;

  // Velocities
  vX = dX / (dt / 1000.0);
  vY = dY / (dt / 1000.0);
  omega = dTheta / (dt / 1000.0);
}

/*****************************************************************************
 *                                 Control                                   *
 *****************************************************************************/
#define MIN_PWM 61
#define FORWARD_MAX 200
#define HEADING_MAX 200
#define TURN_MAX 200

#define FORWARD_P 0
#define FORWARD_I 0
#define FORWARD_D 0
// Theoretical value is 411, but physics is annoying
#define FORWARD_KV 270

#define K_PATH 0
#define HEADING_P 0
#define HEADING_I 0
#define HEADING_D 0

#define TURN_P 0
#define TURN_I 0
#define TURN_D 0.1
#define TURN_KV 1.5

// Actual maximum velocity of the robot is about 0.47 m/s
#define FORWARD_V_MAX 0.35 // Meters per second
#define FORWARD_A_MAX 0.5 // Meters per second squared, this was just tuned

#define TURN_V_MAX 180 // Degrees per second
#define TURN_A_MAX 360 // Degrees per second squared

#define FORWARD_FINISHED_THRESHOLD 0.01 // Meters
#define TURN_FINISHED_THRESHOLD 1 // Degrees

Controller forwardController(FORWARD_P, FORWARD_I, FORWARD_D, FORWARD_MAX);
Controller headingController(HEADING_P, HEADING_I, HEADING_D, HEADING_MAX);
Controller turnController(TURN_P, TURN_I, TURN_D, TURN_MAX);

void driveDistance(double dist) {
  double startX = x;
  double startY = y;
  double startTheta = theta; // Just in case we run into a lot of cte or theta changes small amounts during the movement
  double targetX = x + dist * cos(theta * PI / 180.0);
  double targetY = y + dist * sin(theta * PI / 180.0);

  Trajectory traj(dist, FORWARD_V_MAX, FORWARD_A_MAX);

  unsigned long lastTime = millis();

  double startTime = lastTime / 1000.0;

  // Reset controllers
  forwardController.resetAccumulator();

  // This controller probably won't even have any I gain, but just to be safe
  headingController.resetAccumulator();

  while (fabs(targetX - x) > FORWARD_FINISHED_THRESHOLD || fabs(targetY - y) > FORWARD_FINISHED_THRESHOLD || (millis() / 1000.0) - startTime > traj.T + 2.0) {
    unsigned long currentTime = millis();
    // Approximately 30hz
    if (currentTime - lastTime > 30) {
      // Odometry
      updatePosition();

      // Sample trajectories
      double t = (currentTime / 1000.0) - startTime;
      double desiredPosition = traj.samplePosition(t);
      double desiredVelocity = traj.sampleVelocity(t);

      double traveled = sqrt(pow(x - startX, 2) + pow(y - startY, 2));
      double positionError = desiredPosition - traveled;

      double velocityMagnitude = sqrt(pow(vX, 2) + pow(vY, 2));
      double velocityError = desiredVelocity - velocityMagnitude;

      double feedforward = desiredVelocity * FORWARD_KV;

      // Get commands from controllers
      int driveCmd = forwardController.getCmd(positionError, velocityError) + feedforward;

      if (driveCmd > 0) { driveCmd += MIN_PWM; }
      else if (driveCmd < 0) { driveCmd -= MIN_PWM; }

      // Point to line distance formula
      const double cte = ((targetX - startX) * (startY - y) - (startX - x) * (targetY - startY)) / dist;
      const double desiredTheta = startTheta + (cte * K_PATH);
      const int turnCmd = headingController.getCmd(desiredTheta - theta);

      // Serial.println(x);

      cout << x << endl;

      // We never want to slow down the motors, cause then the forward PID would
      // have to do more work to keep us on trajectory. Because the max output of the
      // forward PID is less than the max PWM, we can always just add the heading cmd
      // to the side that needs to turn so that the total distance we travel stays the same
      int leftPwm, rightPwm;
      if (turnCmd > 0) {
        leftPwm = driveCmd;
        rightPwm = driveCmd + turnCmd;
      } else if (turnCmd < 0) {
        leftPwm = driveCmd - turnCmd;
        rightPwm = driveCmd;
      } else {
        leftPwm = driveCmd;
        rightPwm= driveCmd;
      }

      // Now normalize commands to 0->255 to avoid saturation
      // int maxPwm = max(abs(leftPwm), abs(rightPwm));
      // if (maxPwm > 255) {
      //   const double scaleFactor = 255.0 / maxPwm;
      //   leftPwm *= scaleFactor;
      //   rightPwm *= scaleFactor;
      // }

      // Send commands to motors
      FL_M.run(leftPwm);
      FR_M.run(rightPwm);
      BL_M.run(leftPwm);
      BR_M.run(rightPwm);

      lastTime = currentTime;
    }
  }

  FL_M.stop();
  FR_M.stop();
  BL_M.stop();
  BR_M.stop();
}

void turnAngle(double angle) {
  double startTheta = theta;
  double targetTheta = startTheta + angle;

  Trajectory traj(abs(angle), TURN_V_MAX, TURN_A_MAX);

  unsigned long lastTime = millis();
  double startTime = lastTime / 1000.0;

  turnController.resetAccumulator();

  // Direction multiplier
  double dir = (angle >= 0) ? 1.0 : -1.0;

  while ((millis() / 1000.0) - startTime <= traj.T) {
    unsigned long currentTime = millis();

    if (currentTime - lastTime > 30) {
      updatePosition();

      double t = (currentTime / 1000.0) - startTime;

      // Sample trajectory
      double desiredPos = traj.samplePosition(t) * dir;
      double desiredVel = traj.sampleVelocity(t) * dir;

      double traveledAngle = theta - startTheta;

      double positionError = desiredPos - traveledAngle;
      double velocityError = desiredVel - omega;

      double feedforward = desiredVel * TURN_KV;

      // Get command
      int cmd = turnController.getCmd(positionError, velocityError) + feedforward;

      if (cmd > 0) { cmd += MIN_PWM; }
      else if (cmd < 0) { cmd -= MIN_PWM; }

      // Send to motors
      FL_M.run(-cmd);
      BL_M.run(-cmd);
      FR_M.run(cmd);
      BR_M.run(cmd);

      lastTime = currentTime;
    }
  }

  FL_M.stop();
  FR_M.stop();
  BL_M.stop();
  BR_M.stop();
}

#endif //INC_441SIM_DRIVETRAIN_H
