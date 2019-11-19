#include "editor/ui_interface.h"

#include <imgui.h>
#include <filesystem>

#include "engine/gui/widgets.h"
#include "engine/common/exception.h"
#include "middleware/node_editor/preview_node.h"


static const ImGuiWindowFlags staticWindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse;

static bool BeginWindow(const char* name, const math::Rectf& rect, ImGuiWindowFlags flags = staticWindowFlags) {
    bool* pOpen = nullptr;
    ImGui::SetNextWindowPos(ImVec2(rect.Left(), rect.Top()));
    ImGui::SetNextWindowSize(ImVec2(rect.Width(), rect.Height()));
    return ImGui::Begin(name, pOpen, flags);
}

UIInterface::UIInterface(Engine& engine)
    : m_engine(engine)
    , m_editor("Noise editor") {

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
}

void UIInterface::Render(bool editorMode, const std::shared_ptr<Texture>& texture) {
    auto& gui = m_engine.GetGui();
    auto& wio = m_engine.GetWindow().GetIO();
    uint32_t width, height;
    wio.GetFramebufferSize(width, height);

    gui.NewFrame();

    if (editorMode) {
        math::Rectf rect(0, 0, width, height);

        DrawRightPanel(rect.CutOffRight(300));
        // DrawNodeEditor(rect);
        DrawViewer(rect, texture);

        // ImGui::ShowDemoWindow(nullptr);
        // ImGui::ShowStyleEditor();
    } else {
        DrawInfoBar(math::Rectf(0, 0, 500, 50));
    }

    gui.EndFrame();
}

void UIInterface::Destroy() {
}

void UIInterface::DrawInfoBar(math::Rectf rect) {
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

void UIInterface::DrawRightPanel(math::Rectf rect) {
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

        ImGui::End();
    }
}

void UIInterface::DrawViewer(math::Rectf rect, const std::shared_ptr<Texture>& texture) {
    if (BeginWindow("viewer", rect)) {
        gui::Image(texture, rect.SizeCast<math::Size::Type>());
        ImGui::End();
    }
}

void UIInterface::DrawNodeEditor(math::Rectf rect) {
    static std::shared_ptr<Texture> texture = nullptr;

    math::Rectf viewRect;
    bool drawView = false;
    if (texture) {
        drawView = true;
        viewRect = rect.CutOffTop(rect.Height() / 2);
    }

    if (BeginWindow("node_editor", rect)) {
        m_editor.Draw();
        auto node = m_editor.GetViewNode();
        if (node) {
            auto previewNode = std::dynamic_pointer_cast<PreviewNode>(node);
            if (!previewNode) {
                throw EngineError("Expecting node with type PreviewNode");
            }
            auto view = previewNode->GetView();
            texture = view;
            if (!view) {
                drawView = false;
            }
        } else {
            texture = nullptr;
            drawView = false;
        }

        ImGui::End();
    }

    if (drawView) {
        DrawViewer(viewRect, texture);
    }
}
