//
// Created by Westly Bouchard on 3/12/26.
//

#ifndef INC_441SIM_READABLETIME_H
#define INC_441SIM_READABLETIME_H

#include "Capability.h"

/**
 * Represents the ability for a piece of simulated hardware to be
 * read as a time. This is used only in the base simulator's clock
 * See @link SimulatorBase and @link SimClock
 */
struct ReadableTime : Capability {
    /**
     * Get the current time on the clock
     * @return Time in seconds
     */
    [[nodiscard]] virtual double readTime() const = 0;
};

#endif //INC_441SIM_READABLETIME_H
