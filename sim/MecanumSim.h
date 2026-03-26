//
// Created by west on 3/22/26.
//

#ifndef INC_441SIM_MECANUMSIM_H
#define INC_441SIM_MECANUMSIM_H

#include <array>

#include "SimulatorNew.h"
#include "../hardware/SimMotor.h"
#include "../hardware/SimEncoder.h"

class MecanumSim : public SimulatorNew<6, 4> {
public:
    MecanumSim(double m, double tW, double wB, double wR);

protected:
    void updateHardware() override;
    void setPlantInputs() override;

private:
    const double wheelRadius, wheelBase, trackWidth;

    std::array<std::unique_ptr<SimMotor>, 4> motors;
    std::array<std::unique_ptr<SimEncoder>, 4> encoders;

    [[nodiscard]] std::array<double, 4> fwdKinematics() const;
};


#endif //INC_441SIM_MECANUMSIM_H