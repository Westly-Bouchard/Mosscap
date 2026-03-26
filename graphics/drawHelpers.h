//
// Created by west on 3/25/26.
//

#ifndef INC_441SIM_DRAWHELPERS_H
#define INC_441SIM_DRAWHELPERS_H

#include <imgui.h>

namespace Helpers {
    static double pxPerMeter = 800.0;
    static double scale = 1.0; // Default of 1x1 meter space

    inline void beginVizWindow() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(800, 800));

        ImGui::Begin("Robot", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoResize);
    }

    inline void endVizWindow() {
        ImGui::End();
        ImGui::PopStyleVar(2);
    }

    inline void beginTelemetryWindow() {
        ImGui::SetNextWindowPos(ImVec2(800, 0));
        ImGui::SetNextWindowSize(ImVec2(480, 800));
        ImGui::Begin("Telemetry", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    }

    inline void endTelemetryWindow() {
        ImGui::End();
    }

    inline void setVizScale(const double meters) {
        scale = meters;
        pxPerMeter = 800.0 / meters;
    }

    inline ImVec2 mToPx(const double x, const double y) {
        return {
            static_cast<float>(x * pxPerMeter),
            static_cast<float>((scale - y) * pxPerMeter)
        };
    }

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
        for (auto&& [v, p] : std::views::zip(std::views::as_const(vertices), points)) {
            p.x = static_cast<float>((x + (v.x * c - v.y * s)) * pxPerMeter);
            p.y = static_cast<float>((scale - y - (v.x * s + v.y * c)) * pxPerMeter);
        }

        // Add to draw list
        ImGui::GetWindowDrawList()->AddConvexPolyFilled(points.data(), 4, color);
    }
}

#endif //INC_441SIM_DRAWHELPERS_H