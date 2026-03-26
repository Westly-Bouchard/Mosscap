//
// Created by Westly Bouchard on 3/12/26.
//

#ifndef INC_441SIM_CLOCK_H
#define INC_441SIM_CLOCK_H

#include "../capability/ReadableTime.h"

/**
 * Simulated clock to mimic an Arduino's hardware clock
 */
class SimClock final : public ReadableTime {
public:
    /**
     * Get the current time
     * @return Current time (seconds)
     */
    [[nodiscard]] double readTime() const override;

    /**
     * Update the clock
     * This is called only by the @link Simulator
     * @param dt Time since last update
     */
    void updateTime(double dt);

private:
    double time{0.0};
};

#endif //INC_441SIM_CLOCK_H
