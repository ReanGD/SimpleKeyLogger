#include "engine/gui/gui.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include "engine/material/texture.h"


static void UpdateMouseCursor(Window& window, ImGuiIO& io) {
    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) {
        return;
    }

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (io.MouseDrawCursor) {
        imgui_cursor = ImGuiMouseCursor_None;
    }
    switch (imgui_cursor)
    {
    case ImGuiMouseCursor_None:
        window.SetCursor(CursorType::Hidden);
        break;
    case ImGuiMouseCursor_Arrow:
    case ImGuiMouseCursor_ResizeAll: // FIXME: GLFW doesn't have this.
    case ImGuiMouseCursor_ResizeNESW: // FIXME: GLFW doesn't have this.
    case ImGuiMouseCursor_ResizeNWSE: // FIXME: GLFW doesn't have this.
        window.SetCursor(CursorType::Arrow);
        break;
    case ImGuiMouseCursor_TextInput:
        window.SetCursor(CursorType::IBeam);
        break;
    case ImGuiMouseCursor_ResizeNS:
        window.SetCursor(CursorType::ResizeV);
        break;
    case ImGuiMouseCursor_ResizeEW:
        window.SetCursor(CursorType::ResizeH);
        break;
    case ImGuiMouseCursor_Hand:
        window.SetCursor(CursorType::Hand);
        break;
    default:
        window.SetCursor(CursorType::Arrow);
        break;
    }
}

Gui::~Gui() {
    if (m_context != nullptr) {
        ImGui_ImplOpenGL3_Shutdown();
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
    io.MouseDrawCursor = false;
    io.SetClipboardTextFn = Window::SetClipboardText;
    io.GetClipboardTextFn = Window::GetClipboardText;
    io.ClipboardUserData = nullptr;
    #if defined(_WIN32)
        io.ImeWindowHandle = (void*)glfwGetWin32Window(window);
    #endif

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
    ImGui_ImplOpenGL3_Init("#version 330 core");

    return true;
}

void Gui::EnableInput(bool value) {
    m_enableInput = value;
}

void Gui::Update(Window& window, float deltaTime) {
    WindowInput& wio = window.GetIO();
    ImGuiIO& io = ImGui::GetIO();

    io.DeltaTime = deltaTime;

    uint32_t w, h;
    window.GetWindowSize(w, h);
    io.DisplaySize = ImVec2(static_cast<float>(w), static_cast<float>(h));

    if (w > 0 && h > 0) {
        uint32_t fbWidth, fbHeight;
        wio.GetFramebufferSize(fbWidth, fbHeight);
        io.DisplayFramebufferScale = ImVec2(static_cast<float>(fbWidth) / w, static_cast<float>(fbHeight) / h);
    }

    if (m_enableInput) {
        if (window.IsFocused()) {
            float offsetX, offsetY;
            wio.GetScrollOffset(offsetX, offsetY);
            io.MouseWheelH += offsetX;
            io.MouseWheel += offsetY;

            if (io.WantSetMousePos) {
                window.SetCursorPosition(io.MousePos.x, io.MousePos.y);
            } else {
                float posX, posY;
                wio.GetCursorPosition(posX, posY);
                io.MousePos = ImVec2(posX, posY);
            }
        } else {
            io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
        }

        UpdateMouseCursor(window, io);

        wio.FillKeyboardKeysDown(io.KeysDown);
        io.KeyCtrl = wio.IsKeyDown(Key::Control);
        io.KeyShift = wio.IsKeyDown(Key::Shift);
        io.KeyAlt = wio.IsKeyDown(Key::Alt);
        io.KeySuper = wio.IsKeyDown(Key::Super);
        io.MouseDown[0] = wio.IsKeyStickyDown(Key::MouseLeft);
        io.MouseDown[1] = wio.IsKeyStickyDown(Key::MouseRight);
        io.MouseDown[2] = wio.IsKeyStickyDown(Key::MouseMiddle);

        for(const auto ch :wio.GetInput()) {
            io.AddInputCharacter(ch);
        }
    } else {
        std::fill(std::begin(io.KeysDown), std::end(io.KeysDown), false);

        io.KeyCtrl = false;
        io.KeyShift = false;
        io.KeyAlt = false;
        io.KeySuper = false;
        io.MouseDown[0] = false;
        io.MouseDown[1] = false;
        io.MouseDown[2] = false;
    }
}

void Gui::NewFrame() const {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
}

void Gui::EndFrame() const {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
