//
// Created by Westly Bouchard on 3/12/26.
//

#include "SimClock.h"

SimClock::SimClock() : simTime(0.0) {}

double SimClock::readTime() const {
    return simTime;
}

void SimClock::updateTime(const double dt) {
    simTime += dt;
}