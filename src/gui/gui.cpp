#include "gui/gui.h"

#include <imgui.h>

#include "window/window_input.h"


Gui::~Gui() {
    if (m_context != nullptr) {
        ImGui::DestroyContext(m_context);
        m_context = nullptr;
    }
}

bool Gui::Init(std::string& error) {
    IMGUI_CHECKVERSION();
    m_context = ImGui::CreateContext();
    if (m_context == nullptr) {
        error = "Failed to create ImGui context";
        return false;
    }

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableSetMousePos;
    io.BackendFlags = ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_HasSetMousePos;
    io.BackendPlatformName = "imgui_impl_glfw";

    io.KeyMap[ImGuiKey_Tab] = static_cast<int>(Key::Tab);
    io.KeyMap[ImGuiKey_LeftArrow] = static_cast<int>(Key::ArrowLeft);
    io.KeyMap[ImGuiKey_RightArrow] = static_cast<int>(Key::ArrowRight);
    io.KeyMap[ImGuiKey_UpArrow] = static_cast<int>(Key::ArrowUp);
    io.KeyMap[ImGuiKey_DownArrow] = static_cast<int>(Key::ArrowDown);
    io.KeyMap[ImGuiKey_PageUp] = static_cast<int>(Key::PageUp);
    io.KeyMap[ImGuiKey_PageDown] = static_cast<int>(Key::PageDown);
    io.KeyMap[ImGuiKey_Home] = static_cast<int>(Key::Home);
    io.KeyMap[ImGuiKey_End] = static_cast<int>(Key::End);
    io.KeyMap[ImGuiKey_Insert] = static_cast<int>(Key::Insert);
    io.KeyMap[ImGuiKey_Delete] = static_cast<int>(Key::Delete);
    io.KeyMap[ImGuiKey_Backspace] = static_cast<int>(Key::Backspace);
    io.KeyMap[ImGuiKey_Space] = static_cast<int>(Key::Space);
    io.KeyMap[ImGuiKey_Enter] = static_cast<int>(Key::Enter);
    io.KeyMap[ImGuiKey_Escape] = static_cast<int>(Key::Escape);
    io.KeyMap[ImGuiKey_KeyPadEnter] = static_cast<int>(Key::KeyPadEnter);
    io.KeyMap[ImGuiKey_A] = static_cast<int>(Key::A);
    io.KeyMap[ImGuiKey_C] = static_cast<int>(Key::C);
    io.KeyMap[ImGuiKey_V] = static_cast<int>(Key::V);
    io.KeyMap[ImGuiKey_X] = static_cast<int>(Key::X);
    io.KeyMap[ImGuiKey_Y] = static_cast<int>(Key::Y);
    io.KeyMap[ImGuiKey_Z] = static_cast<int>(Key::Z);

    ImGui::StyleColorsDark();
    return true;
}
