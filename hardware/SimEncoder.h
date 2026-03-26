//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef SIMENCODER_H
#define SIMENCODER_H

#include "../capability/ReadableEncoder.h"

/**
 * Simulated encoder
 */
class SimEncoder final : public ReadableEncoder {
public:
    /**
     * Construct an encoder with given CPR
     * @param CPR Counts per revolution
     */
    explicit SimEncoder(int CPR);

    /**
     * Get the current count
     * @return integer count
     */
    [[nodiscard]] int readCount() const override;

    /**
     * Update the hardware
     * @param offset Angular distance moved since last update
     */
    void updatePosition(double offset);

private:
    /**
     * This amounts to the hardware config
     * Similar to the config of a @link SimMotor
     */
    int CPR;

    /**
     * Current state of the hardware
     */
    double shaftPos;
};

#endif //SIMENCODER_H
