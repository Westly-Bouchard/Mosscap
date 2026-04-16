//
// Created by Westly Bouchard on 4/8/26.
//

#ifndef INC_441SIM_SIMTOF_H
#define INC_441SIM_SIMTOF_H

#include "../capability/ReadableDistance.h"

#include <atomic>

class SimTOF final : public ReadableDistance {
public:
    [[nodiscard]] double readDist() const override;
};



#endif //INC_441SIM_SIMTOF_H
