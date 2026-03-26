//
// Created by west on 3/4/26.
//

#ifndef INC_441SIM_ENCODER_H
#define INC_441SIM_ENCODER_H

#include "../ArduinoRuntime.h"

class Encoder {
public:
    Encoder(const int pinA, int pinB) : index(pinA) {}

    [[nodiscard]] int read() const {
        return ArduinoRuntime::getInstance().getEncoder(index)->readCount();
    }

private:
    int index;

};

#endif //INC_441SIM_ENCODER_H