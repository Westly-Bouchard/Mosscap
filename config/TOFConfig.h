//
// Created by Westly Bouchard on 4/8/26.
//

#ifndef INC_441SIM_TOFCONFIG_H
#define INC_441SIM_TOFCONFIG_H

#include <array>
#include <cmath>
#include <optional>
#include <ranges>

struct Vec {
    double x, y;

    Vec() : x(0.0), y(0.0) {}
    Vec(const double x, const double y) : x(x), y(y) {}
};

struct Object {
    virtual ~Object() = default;

    [[nodiscard]] virtual std::optional<double> intersects(Vec o, Vec d) const = 0;
};

struct Line : Object {
    Vec a, b;

    Line() = default;
    Line(const Vec a, const Vec b) : a(a), b(b) {}

    [[nodiscard]] std::optional<double> intersects(const Vec o, const Vec d) const override {
        const double v2v3 = (b.x - a.x) * (-d.y) + (b.y - a.y) * (d.x);
        const double t1 = (b.x - a.x) * (o.y - a.y) - (b.y - a.y) * (o.x - a.x) / v2v3;
        const double t2 = ((o.x - a.x) * (-d.y) + (o.y - a.y) * (d.x)) / v2v3;

        if (t1 >= 0 && t2 >= 0 && t2 <= 1) {
            // Intersection has occurred, distance can be found with t1
            // Need to find the length of the d vector
            return sqrt(pow(d.x * t1, 2) + pow(d.y * t1, 2));
        }

        return std::nullopt;
    }
};

struct Box : Object {
    std::array<Line, 4> lines;

    Box(const double w, const double h, const Vec pos, const double theta) {
        // Construct lines
        std::array<Vec, 4> points{};

        const double c = cos(theta);
        const double s = sin(theta);

        const std::array<Vec, 4> vertices{{
            {-w / 2.0, -h / 2.0},
            {w / 2.0, -h / 2.0},
            {w / 2.0, h / 2.0},
            {-w / 2.0, h / 2.0}
        }};

        for (auto&& [v, p] : std::views::zip(vertices, points)) {
            p.x = pos.x + (v.x * c - v.y * s);
            p.y = pos.y - (v.x * s + v.y * c);
        }

        lines = {{
            {points.at(0), points.at(1)},
            {points.at(1), points.at(2)},
            {points.at(2), points.at(3)},
            {points.at(3), points.at(0)}
        }};
    }

    [[nodiscard]] std::optional<double> intersects(const Vec o, const Vec d) const override {
        const auto [numIntersections, minDist] = checkLines(o, d);
        // If no intersections return nullopt
        if (numIntersections == 0) { return std::nullopt; }

        // If only one intersection than we're inside the shape and can return zero
        // there's probably some edge case here with corners but whatever
        if (numIntersections == 1) { return 0; }

        // If not then can return the minDistance
        return minDist;
    }

protected:
    [[nodiscard]] std::pair<int, double> checkLines(const Vec o, const Vec d) const {
        int numIntersections{0};
        double minDistance = std::numeric_limits<double>::max();

        for (const auto& l : lines) {
            if (auto res = l.intersects(o, d)) {
                numIntersections++;
                minDistance = std::min(minDistance, *res);
            }
        }

        return std::make_pair(numIntersections, minDistance);
    }
};

/**
 * Special case of box obstacle to represent the bounds of the space that the sensor is in
 * This differs from the regular Box by returning a distance when the sensor is inside it
 * rather than 0 (as if the sensor was pressed up against it)
 */
struct BoundingBox : Box {
    BoundingBox(const double w, const double h) : Box(w, h, Vec(), 0) {}

    // This is a bit funky because if we have 0 intersections than the sensor is outside the
    // bounding box which is kind of a problem. I'm not going to worry about it for now
    [[nodiscard]] std::optional<double> intersects(const Vec o, const Vec d) const override {
        const auto [numIntersections, minDist] = checkLines(o, d);

        if (numIntersections == 0) { return std::nullopt; }

        return minDist;
    }
};

struct TOFConfig {

};

#endif //INC_441SIM_TOFCONFIG_H
