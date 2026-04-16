//
// Created by Westly Bouchard on 4/8/26.
//

#ifndef INC_441SIM_TOFCONFIG_H
#define INC_441SIM_TOFCONFIG_H

#include <optional>

struct Object {
    virtual ~Object() = default;

    virtual std::optional<double> intersects() = 0;
};

struct Line : Object {
    double x, y;

    std::optional<double> intersects() override {
        return std::nullopt;
    }
};

struct TOFConfig {

};

#endif //INC_441SIM_TOFCONFIG_H
