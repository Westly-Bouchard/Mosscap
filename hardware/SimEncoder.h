//
// Created by Westly Bouchard on 3/2/26.
//

#ifndef SIMENCODER_H
#define SIMENCODER_H

#include "../capability/ReadableEncoder.h"

class SimEncoder final : public ReadableEncoder {
public:
    explicit SimEncoder(int CPR);

    int readCount() const override;

    void setCount(int count);

private:
    int CPR, count;
};



#endif //SIMENCODER_H
