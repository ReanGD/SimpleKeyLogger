#include "middleware/node_editor/node_render.h"

#include <cassert>

#include "engine/common/exception.h"
#include "middleware/node_editor/noise_2d.h"
#include "middleware/node_editor/noiseutils.h"


inline uint8_t BlendChannel(const uint8_t channel0, const uint8_t channel1, double alpha) {
    return static_cast<uint8_t>((static_cast<double>(channel1) * alpha) + (static_cast<double>(channel0) * (1.0f - alpha)));
}

inline math::Color LinearInterpColor(const math::Color& color0, const math::Color& color1, double alpha) {
    math::Color out;
    out.alpha = BlendChannel(color0.alpha, color1.alpha, alpha);
    out.blue  = BlendChannel(color0.blue , color1.blue , alpha);
    out.green = BlendChannel(color0.green, color1.green, alpha);
    out.red   = BlendChannel(color0.red  , color1.red  , alpha);

    return out;
}

void GradientColor::Add(double gradientPos, const math::Color& gradientColor) {
    m_points[gradientPos] = gradientColor;
}

const math::Color GradientColor::Get(double gradientPos) const {
    assert(m_points.size() >= 2);

    auto it = m_points.upper_bound(gradientPos);

    if (it == m_points.cbegin()) {
        return it->second;
    } else if (it == m_points.cend()) {
        return m_points.rbegin()->second;
    }
    const double pos1 = it->first;
    const math::Color& color1 = it->second;

    --it;
    const double pos0 = it->first;
    const math::Color& color0 = it->second;

    double alpha = (gradientPos - pos0) / (pos1 - pos0);
    return LinearInterpColor(color0, color1, alpha);
}

void GradientColor::Clear() {
    m_points.clear();
}

RenderNode::RenderNode()
    : PreviewNode("Render")
    , m_render(new noise::utils::RendererImage()) {
    AddInPin(new BasePin(PinType::Noise2D, 0));
    AddOutPin(new BasePin(PinType::Image, 0));
    SetIsFull(false);

    // TODO: set correct size
    m_render->SetDestSize(256, 256);
    m_render->SetBounds(2.0, 6.0, 1.0, 5.0);
}

RenderNode::~RenderNode() {
    if (m_render != nullptr) {
        delete m_render;
        m_render = nullptr;
    }
}

void RenderNode::SetSourceNode(BaseNode* srcNode, BasePin* dstPin) {
    auto* srcNoiseNode = dynamic_cast<BaseNoise2DNode*>(srcNode);
    if (!srcNoiseNode) {
        throw EngineError("BaseNoise2DNode incoming node is expected");
    }

    m_render->SetSourceModule(srcNoiseNode);

    BaseNode::SetSourceNode(srcNode, dstPin);
}

void RenderNode::Update() {
    UpdatePreview(m_render->Render());
}
