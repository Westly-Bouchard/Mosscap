//
// Created by west on 5/6/26.
//

#ifndef INC_441SIM_RENDERER_H
#define INC_441SIM_RENDERER_H

#include <array>
#include <cmath>
#include <functional>
#include <ranges>
#include <utility>
#include <vector>

#include <imgui.h>

class Drawable;

inline double pxPerMeter = 0.0;
inline double scale = 0;

class Renderer {
public:
    static Renderer& getInstance();

    Renderer(const Renderer&) = delete;
    void operator=(const Renderer&) = delete;

    static void setScale(double meters);

    void draw() const;

    void registerDrawable(int layer, Drawable* drawable);
    void registerDrawFunction(int layer, const std::function<void()>& func);

    void unregisterDrawable(Drawable* drawable);

private:
    Renderer() = default;
    ~Renderer() = default;

    std::vector<std::pair<int, Drawable*>> drawables;
    std::vector<std::pair<int, std::function<void()>>> drawFunctions;
};

class Drawable {
public:
    explicit Drawable(const int layer) : l(layer) {
        Renderer::getInstance().registerDrawable(l, this);
    }

    Drawable(const Drawable& other) : l(other.l) {
        Renderer::getInstance().registerDrawable(other.l, this);
    }

    Drawable& operator=(const Drawable& other) {
        if (this != &other) {
            Renderer::getInstance().registerDrawable(other.l, this);
        }

        return *this;
    }

    virtual ~Drawable() {
        Renderer::getInstance().unregisterDrawable(this);
    }

    virtual void draw() const = 0;

protected:
    static void drawRect(const double x, const double y, const double theta, const double w,
                     const double h, const ImColor color) {
        std::array<ImVec2, 4> points;
        const double c = cos(theta);
        const double s = sin(theta);

        // Center relative corner locations, before rotation
        // Why does imgui use floats? So many casts...
        const std::array<ImVec2, 4> vertices = {{
            {static_cast<float>(-1.0 * w / 2.0), static_cast<float>(-1.0 * h / 2.0)},
            {static_cast<float>(w / 2.0), static_cast<float>(-1.0 * h / 2.0)},
            {static_cast<float>(w / 2.0), static_cast<float>(h / 2.0)},
            {static_cast<float>(-1.0 * w / 2.0), static_cast<float>(h / 2.0)}
        }};

        // Rotate and translate each corner
        for (auto&& [v, p] : std::views::zip(vertices, points)) {
            p.x = static_cast<float>((x + (v.x * c - v.y * s)) * pxPerMeter);
            p.y = static_cast<float>((scale - y - (v.x * s + v.y * c)) * pxPerMeter);
        }

        // Add to draw list
        ImGui::GetWindowDrawList()->AddConvexPolyFilled(points.data(), 4, color);
    }

private:
    int l;
};

#endif //INC_441SIM_RENDERER_H
