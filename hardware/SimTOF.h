//
// Created by Westly Bouchard on 4/8/26.
//

#ifndef INC_441SIM_SIMTOF_H
#define INC_441SIM_SIMTOF_H

#include <atomic>

#include "../capability/ReadableDistance.h"
#include "../config/TOFConfig.h"

class SimTOF final : public ReadableDistance {
public:
    explicit SimTOF(TOFConfig  config);
    /**
     * Get the current reading, this is called by the Arduino runtime
     * through a handle to this hardware's capability
     * @return Current distance read by the sensor
     */
    [[nodiscard]] double readDist() const override;

    /**
     * Recalculate sensor measurement
     * @param x X position of the chassis center
     * @param y Y position of the chassis center
     * @param theta Chassis angle from positive x-axis
     */
    void update(double x, double y, double theta);

    /**
     * This does effectively the same thing as `readDist()` but
     * this method is not part of the hardware's capability. This method
     * is only intended to be called by the simulator itself.
     * @return Current sensor reading
     */
    [[nodiscard]] double getDist() const;

private:
    const TOFConfig config;

    std::atomic<double> dist;
};



#endif //INC_441SIM_SIMTOF_H
