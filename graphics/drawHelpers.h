//
// Created by west on 3/25/26.
//

#ifndef INC_441SIM_DRAWHELPERS_H
#define INC_441SIM_DRAWHELPERS_H

#include <array>

#include <imgui.h>

/**
 * Collection of useful helper functions for drawing system
 * state to the simulation window.
 */
namespace Helpers {
    /**
     * Space for drivable robots defaults to 1x1 meters
     */
    inline double pxPerMeter = 800.0;
    inline double scale = 1.0;

    /**
     * Set up visualization window (for drawing robot state / position)
     */
    inline void beginVizWindow() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(800, 800));

        ImGui::Begin("Robot", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoResize);
    }

    /**
     * End visualization window
     * This really just wraps a couple of ImGui calls
     */
    inline void endVizWindow() {
        ImGui::End();
        ImGui::PopStyleVar(2);
    }

    /**
     * Set up telemetry (or debug) window
     */
    inline void beginTelemetryWindow() {
        ImGui::SetNextWindowPos(ImVec2(800, 0));
        ImGui::SetNextWindowSize(ImVec2(480, 800));
        ImGui::Begin("Telemetry", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    }

    /**
     * End telemetry window
     * Again, this just wraps an ImGui call
     */
    inline void endTelemetryWindow() {
        ImGui::End();
    }

    /**
     * Set the scale for the visualization window
     * @param meters Side length of the viz window min meters
     */
    inline void setVizScale(const double meters) {
        scale = meters;
        pxPerMeter = 800.0 / meters;
    }

    /**
     * Convert a point represented in meters to one represented in pixels
     * @param x position in meters
     * @param y position in meters
     * @return point in pixel space
     */
    inline ImVec2 mToPx(const double x, const double y) {
        return {
            static_cast<float>(x * pxPerMeter),
            static_cast<float>((scale - y) * pxPerMeter)
        };
    }

    /**
     * Draw a rectangle with the given parameters
     * @param x position of center (meters)
     * @param y position of center (meters)
     * @param theta orientation (rad) measured from x+
     * @param w width (meters)
     * @param h height (meters)
     * @param color color
     */
    inline void drawRect(const double x, const double y, const double theta, const double w,
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
}

#endif //INC_441SIM_DRAWHELPERS_H