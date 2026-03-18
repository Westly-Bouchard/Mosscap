//
// Created by Westly Bouchard on 3/12/26.
//

#ifndef INC_441SIM_CLOCK_H
#define INC_441SIM_CLOCK_H

#include "../capability/ReadableTime.h"

class SimClock final : public ReadableTime {
public:
    SimClock();

    [[nodiscard]] double readTime() const override;

    void updateTime(double dt);

private:
    double simTime;
};



#endif //INC_441SIM_CLOCK_H
