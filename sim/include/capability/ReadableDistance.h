//
// Created by Westly Bouchard on 4/8/26.
//

#ifndef INC_441SIM_READABLEDISTANCE_H
#define INC_441SIM_READABLEDISTANCE_H

#include "Capability.h"

/**
 * Represents the ability of a piece of simulated hardware to
 * be read as a distance. This is currently used in the TOF hardware
 */
struct ReadableDistance : Capability {
    /**
     * Get the distance provided by this capability
     * @return Distance in meters
     */
    [[nodiscard]] virtual double readDist() const = 0;
};

#endif //INC_441SIM_READABLEDISTANCE_H
