//
// Created by Westly Bouchard on 3/2/26.
//

#include "SimEncoder.h"

#include <cmath>

SimEncoder::SimEncoder(const int CPR) : CPR(CPR), shaftPos(0) {}

int SimEncoder::readCount() const {
    // Only convert when read so we don't lose precision
    return floor((shaftPos / (2 * M_PI)) * CPR);
}

void SimEncoder::updatePosition(const double offset) {
    shaftPos += offset;
}