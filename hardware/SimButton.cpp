//
// Created by Westly Bouchard on 3/2/26.
//

#include "SimButton.h"

bool SimButton::digitalRead() const {
    return state;
}

void SimButton::setState(const bool state) {
    this->state = state;
}

