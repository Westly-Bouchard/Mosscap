//
// Created by west on 5/13/26.
//

#ifndef MOSSCAP_TANKPLANT_H
#define MOSSCAP_TANKPLANT_H

#include "Plant.hpp"
#include "config/WMRConfig.h"

class TankPlant final : public Plant<6, 2> {
public:
    explicit TankPlant(const WMRConfig &config);

    void operator() (const state_t &x, state_t &dxdt, double t) const override;

private:
    const WMRConfig& config;
};



#endif //MOSSCAP_TANKPLANT_H
