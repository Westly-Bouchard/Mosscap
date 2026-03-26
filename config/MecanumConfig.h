//
// Created by west on 3/22/26.
//

#ifndef INC_441SIM_MECANUMCONFIG_H
#define INC_441SIM_MECANUMCONFIG_H

#include <cmath>

struct MecanumConfig {
    double mass;
    double trackWidth;
    double wheelBase;
    double wheelRadius;

    [[nodiscard]] double getInertia() const {
        return 1.0 / 12.0 * mass * (pow(trackWidth, 2) + pow(wheelBase, 2));
    }
};

#endif //INC_441SIM_MECANUMCONFIG_H