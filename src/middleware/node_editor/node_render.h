#pragma once

#include <map>

#include "engine/common/math.h"
#include "middleware/node_editor/preview_node.h"


class GradientColor {
public:
    GradientColor() = default;

    void Add(double gradientPos, const math::Color& gradientColor);
    size_t Count() const noexcept { return m_points.size(); }
    const math::Color Get(double gradientPos) const;
    void Clear();

private:
    // pos => color
    std::map<double, math::Color> m_points;
};

class RenderNode : public PreviewNode {
public:
    RenderNode();
    ~RenderNode() override;

    void SetSourceNode(BaseNode* srcNode, BasePin* dstPin) final;
    void Update() final;

protected:
    bool DrawSettings() final { return false; }

private:
    noise::utils::RendererImage* m_render = nullptr;
};