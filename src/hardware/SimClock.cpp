//
// Created by Westly Bouchard on 3/12/26.
//

#include "hardware/SimClock.h"

double SimClock::readTime() const {
    return time;
}

void SimClock::updateTime(const double dt) {
    time += dt;
}