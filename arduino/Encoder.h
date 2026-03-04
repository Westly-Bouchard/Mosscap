//
// Created by Westly Bouchard on 3/4/26.
//

#ifndef INC_441SIM_ENCODER_H
#define INC_441SIM_ENCODER_H

#include "../ArduinoRuntime.h"

class Encoder {
public:
    Encoder(const int pinA, int pinB) : indexPin(pinA), offset(0) {}

    [[nodiscard]] int read() const {
        return getCount() - offset;
    }

    int readAndReset() {
        const auto count = getCount();
        const auto ret = count - offset;
        offset = count;
        return ret;
    }

    void write(const int count) {
        offset = getCount() - count;
    }

private:
    [[nodiscard]] int getCount() const {
        return ArduinoRuntime::getInstance().getEncoder(indexPin)->readCount();
    }

    int indexPin;
    int offset;
};

#endif //INC_441SIM_ENCODER_H
