#pragma once

#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "engine/window/window.h"


class Texture;
struct ImGuiContext;
class Gui : Noncopyable {
public:
    Gui() = default;
    ~Gui();

public:
    bool Init(std::string& error);
    void EnableInput(bool value);
    void Update(Window& window, float deltaTime);

    void NewFrame() const;
    void EndFrame() const;

    void Image(const std::shared_ptr<Texture>& texture, const glm::vec2& size,
        const glm::vec2& uv0 = glm::vec2(0),
        const glm::vec2& uv1 = glm::vec2(1),
        const glm::vec4& tintCol = glm::vec4(1),
        const glm::vec4& borderCol = glm::vec4(0));
private:
    bool m_enableInput = true;
    ImGuiContext* m_context = nullptr;
};
