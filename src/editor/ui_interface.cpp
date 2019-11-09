#include "editor/ui_interface.h"

#include <imgui.h>
#include <filesystem>

#include "engine/gui/widgets.h"
#include "editor/ui_node_editor.h"
#include "engine/heightmap/heightmap.h"
#include "engine/common/exception.h"


static const ImGuiWindowFlags staticWindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse;

static bool BeginWindow(const char* name, rect& rect, ImGuiWindowFlags flags = staticWindowFlags) {
    bool* pOpen = nullptr;
    ImGui::SetNextWindowPos(ImVec2(rect.posX, rect.posY));
    ImGui::SetNextWindowSize(ImVec2(rect.sizeX, rect.sizeY));
    return ImGui::Begin(name, pOpen, flags);
}

UIInterface::UIInterface(Engine& engine)
    : m_engine(engine) {

}

void UIInterface::Init() {
    ImGuiIO& io = ImGui::GetIO();
    m_fontDefault = io.Fonts->AddFontDefault();
    if (m_fontDefault == nullptr) {
        throw EngineError("Failed to load a default font");
    }

    const auto monoFontPath = std::filesystem::current_path() / "data" / "fonts" / "NotoSans" / "NotoSans-Regular.ttf";
    m_fontMono = io.Fonts->AddFontFromFileTTF(monoFontPath.c_str(), 26, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    if (m_fontMono == nullptr) {
        throw EngineError("Failed to load a font from file '{}'", monoFontPath.c_str());
    }

    m_nodeEditor = std::make_shared<UINodeEditor>();
    m_nodeEditor->Create();

    m_heightmap = std::make_shared<Heightmap>();
    m_heightmap->Create();
}

void UIInterface::Render(bool editorMode) {
    auto& gui = m_engine.GetGui();
    auto& wio = m_engine.GetWindow().GetIO();
    uint32_t width, height;
    wio.GetFramebufferSize(width, height);

    gui.NewFrame();
    rect rect;

    if (editorMode) {
        uint32_t widthRightPanel = 300;
        rect.sizeX = widthRightPanel;
        rect.sizeY = height;
        rect.posX = width - rect.sizeX;
        rect.posY = 0;
        DrawRightPanel(rect);

        rect.sizeX = width - widthRightPanel;
        rect.sizeY = height;
        rect.posX = 0;
        rect.posY = 0;
        // DrawViewer(rect);
        DrawNodeEditor(rect);
        // ImGui::ShowDemoWindow(nullptr);
        // ImGui::ShowStyleEditor();
    } else {
        rect.posX = 0;
        rect.posY = 0;
        rect.sizeX = 500;
        rect.sizeY = 50;
        DrawInfoBar(rect);
    }

    gui.EndFrame();
}

void UIInterface::Destroy() {
    if (m_nodeEditor) {
        m_nodeEditor->Destroy();
        m_nodeEditor.reset();
    }
}

void UIInterface::DrawInfoBar(rect& rect) {
    if (BeginWindow("infobar", rect)) {
        ImGui::PushFont(m_fontMono);
        auto pos = m_engine.GetScene().GetCamera()->GetPosition();
        auto text = fmt::format("FPS = {:.1f} TPF = {:.2f}M Pos = {:.1f}:{:.1f}:{:.1f}",
            m_engine.GetFps(),
            static_cast<double>(m_engine.GetTpf()) / 1000.0 / 1000.0,
            pos.x, pos.y, pos.z);
        ImGui::TextColored(ImColor(0xFF, 0xDA, 0x00), "%s", text.c_str());
        ImGui::PopFont();

        ImGui::End();
    }
}

void UIInterface::DrawRightPanel(rect& rect) {
    if (BeginWindow("right_panel", rect)) {

        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_None)) {
            auto camera = m_engine.GetScene().GetCamera();
            float nearPlane = camera->GetNearPlane();
            if (ImGui::DragFloat("near plane", &nearPlane, 0.1f, 0.1f, 10.0f, "%.1f")) {
                camera->SetNearPlane(nearPlane);
            }

            float farPlane = camera->GetFarPlane();
            if (ImGui::DragFloat("far plane", &farPlane, 1.0f, 10.0f, 5000.0f, "%.0f")) {
                camera->SetFarPlane(farPlane);
            }
        }

        if (ImGui::CollapsingHeader("Heightmap", ImGuiTreeNodeFlags_DefaultOpen)) {
            m_heightmap->DrawSettings();
        }

        ImGui::End();
    }
}

void UIInterface::DrawViewer(rect& rect) {
    if (BeginWindow("viewer", rect)) {
        gui::Image(m_heightmap->GetPreview(), math::Size(1024,1024), math::Pointf(0,1), math::Pointf(1,0));
        ImGui::End();
    }
}

void UIInterface::DrawNodeEditor(rect& rect) {
    if (BeginWindow("node_editor", rect)) {
        m_nodeEditor->Draw();
        ImGui::End();
    }
}
