//
// Created by Westly Bouchard on 4/2/26.
//

#ifndef INC_441SIM_CONTROLLER_H
#define INC_441SIM_CONTROLLER_H

class Controller {
public:
    double kP, kI, kD, maxCmd, acc;
    unsigned long lastTime;
    double lastError;

    Controller(double kP, double kI, double kD, double maxCmd)
      : kP(kP), kI(kI), kD(kD), maxCmd(maxCmd), acc(0.0), lastTime(millis()), lastError(0.0) {}

    double getCmd(double posError, double velError) {
        double p = posError * kP;
        double d = velError * kD;

        unsigned long currentTime = millis();
        int dt = currentTime - lastTime;
        lastTime = currentTime;

        acc += posError * dt / 1000.0;
        double i = acc * kP;

        double cmd = constrain(p + i + d, -maxCmd, maxCmd);
        return cmd;
    }

    double getCmd(double posError) {
        double p = posError * kP;

        double dError = posError - lastError;
        lastError = posError;
        double d = dError * kD;

        unsigned long currentTime = millis();
        int dt = currentTime - lastTime;
        lastTime = currentTime;

        acc += posError * dt / 1000.0;
        double i = acc * kP;

        double cmd = constrain(p + i + d, -maxCmd, maxCmd);
        // return cmd;
        return p + i + d;
    }

    void resetAccumulator() {
        acc = 0.0;
    }
};

#endif //INC_441SIM_CONTROLLER_H
