//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef SIMENCODER_H
#define SIMENCODER_H

#include "../capability/ReadableEncoder.h"

class SimEncoder final : public ReadableEncoder {
public:
    explicit SimEncoder(int CPR);

    [[nodiscard]] int readCount() const override;

    void updatePosition(double offset);

private:
    int CPR;
    double shaftPos;
};



#endif //SIMENCODER_H
