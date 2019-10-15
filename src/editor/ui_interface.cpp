#include "editor/ui_interface.h"

#include <imgui.h>
#include <filesystem>
#include <fmt/format.h>
#include <imgui_node_editor.h>
#include "engine/heightmap/heightmap.h"
#include "engine/gui/widgets.h"


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
        m_engine.GetGui().Image(m_heightmap->GetPreview(), glm::vec2(1024,1024), glm::vec2(0,1), glm::vec2(1,0));
        ImGui::End();
    }
}

struct LinkInfo
{
    ed::LinkId Id;
    ed::PinId  InputId;
    ed::PinId  OutputId;
};

static ImVector<LinkInfo>   g_Links;
static int                  g_NextLinkId = 100;
void UIInterface::DrawNodeEditor(rect& rect) {
    if (BeginWindow("node_editor", rect)) {
        ed::SetCurrentEditor(g_Context);

        ed::Begin("My Editor");

        unsigned long uniqueId = 1;


        // Node A
        ed::NodeId nodeA_Id = uniqueId++;
        ed::PinId  nodeA_InputPinId = uniqueId++;
        ed::PinId  nodeA_OutputPinId = uniqueId++;

        ed::BeginNode(nodeA_Id);
            ImGui::Text("Node A");
            ed::BeginPin(nodeA_InputPinId, ed::PinKind::Input);
                ImGui::Text("-> In");
            ed::EndPin();
            ImGui::SameLine();
            ed::BeginPin(nodeA_OutputPinId, ed::PinKind::Output);
                ImGui::Text("Out ->");
            ed::EndPin();
        ed::EndNode();

        // Node B
        ed::NodeId nodeB_Id = uniqueId++;
        ed::PinId  nodeB_InputPinId1 = uniqueId++;
        ed::PinId  nodeB_InputPinId2 = uniqueId++;
        ed::PinId  nodeB_OutputPinId = uniqueId++;

        ed::BeginNode(nodeB_Id);
            ImGui::Text("Node B");
            // ImGuiEx_BeginColumn();
            ImGui::BeginGroup();
                ed::BeginPin(nodeB_InputPinId1, ed::PinKind::Input);
                    ImGui::Text("-> In1");
                ed::EndPin();
                ed::BeginPin(nodeB_InputPinId2, ed::PinKind::Input);
                    ImGui::Text("-> In2");
                ed::EndPin();
            // ImGuiEx_NextColumn();
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            //
                ed::BeginPin(nodeB_OutputPinId, ed::PinKind::Output);
                    ImGui::Text("Out ->");
                ed::EndPin();
            // ImGuiEx_EndColumn();
            ImGui::EndGroup();
        ed::EndNode();

    // Submit Links
    for (auto& linkInfo : g_Links) {
        ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);
    }

// Handle creation action, returns true if editor want to create new object (node or link)
    if (ed::BeginCreate())
    {
        ed::PinId inputPinId, outputPinId;
        if (ed::QueryNewLink(&inputPinId, &outputPinId))
        {
            // QueryNewLink returns true if editor want to create new link between pins.
            //
            // Link can be created only for two valid pins, it is up to you to
            // validate if connection make sense. Editor is happy to make any.
            //
            // Link always goes from input to output. User may choose to drag
            // link from output pin or input pin. This determine which pin ids
            // are valid and which are not:
            //   * input valid, output invalid - user started to drag new ling from input pin
            //   * input invalid, output valid - user started to drag new ling from output pin
            //   * input valid, output valid   - user dragged link over other pin, can be validated

            if (inputPinId && outputPinId) // both are valid, let's accept link
            {
                // ed::AcceptNewItem() return true when user release mouse button.
                if (ed::AcceptNewItem())
                {
                    // Since we accepted new link, lets add one to our list of links.
                    g_Links.push_back({ ed::LinkId(static_cast<uintptr_t>(g_NextLinkId++)), inputPinId, outputPinId });

                    // Draw new link.
                    ed::Link(g_Links.back().Id, g_Links.back().InputId, g_Links.back().OutputId);
                }

                // You may choose to reject connection between these nodes
                // by calling ed::RejectNewItem(). This will allow editor to give
                // visual feedback by changing link thickness and color.
            }
        }
    }

       // Handle deletion action
    if (ed::BeginDelete())
    {
        // There may be many links marked for deletion, let's loop over them.
        ed::LinkId deletedLinkId;
        while (ed::QueryDeletedLink(&deletedLinkId))
        {
            // If you agree that link can be deleted, accept deletion.
            if (ed::AcceptDeletedItem())
            {
                // Then remove link from your data.
                for (auto& link : g_Links)
                {
                    if (link.Id == deletedLinkId)
                    {
                        g_Links.erase(&link);
                        break;
                    }
                }
            }

            // You may reject link deletion by calling:
            // ed::RejectDeletedItem();
        }
    }
    ed::EndDelete(); // Wrap up deletion action

    ed::EndCreate(); // Wraps up object creation action handling.

        ed::End();

        ImGui::End();
    }
}
