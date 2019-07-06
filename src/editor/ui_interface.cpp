#include "editor/ui_interface.h"

#include <imgui.h>
#include <filesystem>
#include <fmt/format.h>


bool UIInterface::Init(const Gui&, std::string& error) {
    ImGuiIO& io = ImGui::GetIO();
    m_fontDefault = io.Fonts->AddFontDefault();
    if (m_fontDefault == nullptr) {
        error = "Failed to load a default font";
        return false;
    }

    const auto monoFontPath = std::filesystem::current_path() / "data" / "fonts" / "NotoSans" / "NotoSans-Regular.ttf";
    m_fontMono = io.Fonts->AddFontFromFileTTF(monoFontPath.c_str(), 26, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    if (m_fontMono == nullptr) {
        error = fmt::format("Failed to load a font from file '{}'", monoFontPath.c_str());
        return false;

    }
    return true;
}

void UIInterface::DrawInfoBar(float fps) {
    bool* pOpen = nullptr;
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(300, 50));
    if (ImGui::Begin("infobar", pOpen, windowFlags)){
        ImGui::PushFont(m_fontMono);
        ImGui::TextColored(ImColor(0xFF, 0xDA, 0x00), "FPS = %.2f", fps);
        ImGui::PopFont();
        ImGui::End();
    }
}

void UIInterface::DrawExample() {
    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    static bool m_showDemoWindow = false;
    static bool m_showAnotherWindow = false;

    if (m_showDemoWindow) {
        ImGui::ShowDemoWindow(&m_showDemoWindow);
    }

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Win1");

        ImGui::Text("This is some useful text.");
        ImGui::Checkbox("Demo Window", &m_showDemoWindow);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &m_showAnotherWindow);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", reinterpret_cast<float*>(&clearColor)); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    if (m_showAnotherWindow) {
        ImGui::Begin("Another Window", &m_showAnotherWindow);
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            m_showAnotherWindow = false;
        ImGui::End();
    }
}

void UIInterface::Draw(const Gui& gui, float fps) {
    gui.NewFrame();
    DrawInfoBar(fps);
    // DrawExample();
    // ImGui::ShowStyleEditor();

    gui.EndFrame();
}
