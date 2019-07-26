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

void UIInterface::Draw(Engine& engine, bool editorMode) {
    auto& gui = engine.GetGui();
    gui.NewFrame();
    DrawInfoBar(engine);
    if (editorMode) {
        DrawRightPanel(engine);
        // DrawExample();
        // ImGui::ShowStyleEditor();
    }

    gui.EndFrame();
}


void UIInterface::DrawInfoBar(Engine& engine) {
    bool* pOpen = nullptr;
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(300, 50));
    if (ImGui::Begin("infobar", pOpen, windowFlags)){
        ImGui::PushFont(m_fontMono);
        auto text = fmt::format("FPS = {:.1f} TPF = {:.2f}M", engine.GetFps(), static_cast<double>(engine.GetTpf()) / 1000.0 / 1000.0);
        ImGui::TextColored(ImColor(0xFF, 0xDA, 0x00), "%s", text.c_str());
        ImGui::PopFont();
        ImGui::End();
    }
}

void UIInterface::DrawRightPanel(Engine& engine) {
    auto camera = engine.GetScene().GetCamera();
    auto& wio = engine.GetWindow().GetIO();
    uint32_t fbWidth, fbHeight;
    wio.GetFramebufferSize(fbWidth, fbHeight);

    bool* pOpen = nullptr;
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse;

    uint32_t width = 300;
    uint32_t height = fbHeight;
    ImGui::SetNextWindowPos(ImVec2(fbWidth - width, 0));
    ImGui::SetNextWindowSize(ImVec2(width, height));
    if (ImGui::Begin("right_panel", pOpen, windowFlags)){

        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
            float nearPlane = camera->GetNearPlane();
            if (ImGui::DragFloat("near plane", &nearPlane, 0.1f, 0.1f, 10.0f, "%.1f")) {
                camera->SetNearPlane(nearPlane);
            }

            float farPlane = camera->GetFarPlane();
            if (ImGui::DragFloat("far plane", &farPlane, 1.0f, 10.0f, 5000.0f, "%.0f")) {
                camera->SetFarPlane(farPlane);
            }
        }

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
