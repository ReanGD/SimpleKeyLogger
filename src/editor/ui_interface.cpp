#include "editor/ui_interface.h"

#include <imgui.h>
#include <filesystem>
#include <fmt/format.h>
#include <imgui_node_editor.h>
#include "engine/heightmap/heightmap.h"


namespace ed = ax::NodeEditor;

static ed::EditorContext* g_Context = nullptr;

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

bool UIInterface::Init(std::string& error) {
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

    g_Context = ed::CreateEditor();

    m_heightmap = std::make_shared<Heightmap>();
    if (!m_heightmap->Create(error)) {
        return false;
    }

    return true;
}

void UIInterface::Render(bool editorMode, uint image) {
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
        DrawViewer(rect, image);
        // DrawExample();
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
    ed::DestroyEditor(g_Context);
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

        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
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

        ImGui::End();
    }
}

void UIInterface::DrawViewer(rect& rect, uint /*image*/) {
    if (BeginWindow("viewer", rect)) {
        m_engine.GetGui().Image(m_heightmap->GetPreview(), glm::vec2(1024,1024), glm::vec2(0,1), glm::vec2(1,0));
        ImGui::End();
    }
}

void UIInterface::DrawNodeEditor(rect& rect) {
    if (BeginWindow("node_editor", rect)) {
        ed::SetCurrentEditor(g_Context);

        ed::Begin("My Editor");

        unsigned long uniqueId = 1;

        // Start drawing nodes.
        ed::BeginNode(uniqueId++);
            ImGui::Text("Node A");
            ed::BeginPin(uniqueId++, ed::PinKind::Input);
                ImGui::Text("-> In");
            ed::EndPin();
            ImGui::SameLine();
            ed::BeginPin(uniqueId++, ed::PinKind::Output);
                ImGui::Text("Out ->");
            ed::EndPin();
        ed::EndNode();

        ed::End();

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
