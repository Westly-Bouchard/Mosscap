//
// Created by west on 3/4/26.
//

#ifndef INC_441SIM_ENCODER_H
#define INC_441SIM_ENCODER_H

#include "ArduinoRuntime.h"

/**
 * Encoder on a robot
 */
class Encoder {
public:
    /**
     * Construct an encoder object
     * For sim purposes only pinA is important
     * @param pinA pin
     * @param pinB pin
     */
    Encoder(const int pinA, int pinB) : index(pinA), offset(0) {}

    /**
     * Read the current count on the encoder
     * @return Current count
     */
    [[nodiscard]] int read() const {
        return ArduinoRuntime::getInstance().getEncoder(index).get().readCount() - offset;
    }

    [[nodiscard]] int readAndReset() {
        const int count = ArduinoRuntime::getInstance().getEncoder(index).get().readCount();
        const int ret = count - offset;
        offset = count;
        return ret;

    }

private:
    /**
     * Identifier to get the correct handle from the simulator when reading
     */
    int index;

    int offset;

};

#endif //INC_441SIM_ENCODER_H