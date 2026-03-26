//
// Created by west on 3/25/26.
//

#ifndef INC_441SIM_DRAWHELPERS_H
#define INC_441SIM_DRAWHELPERS_H

#include <imgui.h>

namespace Helpers {
    inline void beginVizWindow() {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(800, 800));

        ImGui::Begin("", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoResize);
    }

    inline void endVizWindow() {
        ImGui::End();
    }

    inline void beginTelemetryWindow() {
        ImGui::SetNextWindowPos(ImVec2(800, 0));
        ImGui::SetNextWindowSize(ImVec2(480, 800));
        ImGui::Begin("Telemetry", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    }

    inline void endTelemetryWindow() {
        ImGui::End();
    }
}

#endif //INC_441SIM_DRAWHELPERS_H