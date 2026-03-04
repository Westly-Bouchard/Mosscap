//
// Created by Westly Bouchard on 3/2/26.
//

#include "SimEncoder.h"

SimEncoder::SimEncoder(const int CPR) : CPR(CPR), count(0) {
}

int SimEncoder::readCount() const {
    return count;
}

void SimEncoder::setCount(const int count) {
    this->count = count;
}