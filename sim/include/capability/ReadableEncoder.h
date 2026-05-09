//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef READABLEENCODER_H
#define READABLEENCODER_H

#include "Capability.h"

/**
 * Represents the ability of a piece of simulated hardware to
 * be read as an encoder with integer number of counts.
 */
struct ReadableEncoder : Capability {
    /**
     * Get the number of counts accumulated by the encoder
     * @return Integral number of counts
     */
    [[nodiscard]] virtual int readCount() const = 0;
};

#endif //READABLEENCODER_H
