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

// Forward declare
class Drawable;

// Useful constants to have while rendering
inline double pxPerMeter = 0.0;
inline double scale = 0;

/**
 * Singleton that manages all drawing to the ImGui
 * visualization window.
 */
class Renderer {
public:
    /**
     * Singleton
     * @return Renderer instance
     */
    static Renderer& getInstance();

    /**
     * Deleted because singleton
     */
    Renderer(const Renderer&) = delete;
    void operator=(const Renderer&) = delete;

    /**
     * Set the rendering scale.
     * This is intended to be called by the user at some point in Sim.h
     * @param meters Side length of visualization square in meters
     */
    static void setScale(double meters);

    /**
     * Draw all objects
     */
    void draw() const;

    /**
     * Register a drawable object with the Renderer.
     * The robot renders to layer zero, so anything below the robot should use
     * a negative layer and anything to be superimposed should use a positive
     * layer.
     * @param layer Layer to draw on
     * @param drawable Pointer to obj that implements Drawable
     */
    void registerDrawable(int layer, Drawable* drawable);

    /**
     * Register a function to be called at render time.
     * This is useful for drawing things that don't directly relate to the robot
     * or hardware.
     * The robot renders to layer zero, so anything below the robot should use
     * a negative layer and anything to be superimposed should use a positive
     * layer.
     * @param layer Layer to draw on
     * @param func Callable block of code
     */
    void registerDrawFunction(int layer, const std::function<void()>& func);

    /**
     * This provides a method for a drawable object that goes out of scope
     * to remove itself from the draw list.
     * @param drawable Drawable object to remove
     */
    void unregisterDrawable(Drawable* drawable);

private:
    /**
     * Private because singleton
     */
    Renderer() = default;
    ~Renderer() = default;

    /**
     * Vectors of things to draw, be they objects or functions to call
     */
    std::vector<std::pair<int, Drawable*>> drawables;
    std::vector<std::pair<int, std::function<void()>>> drawFunctions;
};

/**
 * Interface for objects to implement that allows them to draw something
 * to the ImGui visualization window.
 */
class Drawable {
public:
    /**
     * Implementer must specify drawing layer
     * @param layer Layer to draw to
     */
    explicit Drawable(const int layer) : l(layer) {
        Renderer::getInstance().registerDrawable(l, this);
    }

    /**
     * Copy constructor and copy assignment to ensure that copied objects
     * will still be drawn, even if the original drawable object goes out
     * of scope.
     */
    Drawable(const Drawable& other) : l(other.l) {
        Renderer::getInstance().registerDrawable(other.l, this);
    }

    Drawable& operator=(const Drawable& other) {
        if (this != &other) {
            Renderer::getInstance().registerDrawable(other.l, this);
        }

        return *this;
    }

    /**
     * Dtor unregisters self from Renderer
     */
    virtual ~Drawable() {
        Renderer::getInstance().unregisterDrawable(this);
    }

    /**
     * Draw function to be implemented by derived classes
     */
    virtual void draw() const = 0;

protected:
    /**
     * Helper function to draw a rectangle to draw a rectangle to the
     * visualization window.
     * @param x X position of rect center
     * @param y Y position of rect center
     * @param theta Angle of rect from +x-axis
     * @param w Width of rect
     * @param h Height of rect
     * @param color Color
     */
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
