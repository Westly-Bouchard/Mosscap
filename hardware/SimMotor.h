//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef SIMMOTOR_H
#define SIMMOTOR_H

#include "../capability/WriteablePWM.h"

class SimMotor final : public WriteablePWM {
public:
    SimMotor(double kV, double kT, double R, double vBus);

    void writePWM(int value) override;

    double getTorque(double speed) const;

private:
    double kV, kT, R, vBus;

    int pwm;
};



#endif //SIMMOTOR_H
