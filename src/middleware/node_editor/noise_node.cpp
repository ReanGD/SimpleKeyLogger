#include "middleware/node_editor/noise_node.h"

#include <noise/noiseutils.h>
#include <imgui_node_editor.h>
#include <imgui_internal.h>

#include "engine/gui/widgets.h"
#include "engine/material/texture_manager.h"


namespace ne = ax::NodeEditor;

static const char* QualityItems[] = {"Fast", "Std", "Best"};

BaseNoiseNode::BaseNoiseNode(const noise::module::Module* module, const std::string& name)
    : BaseNode(name)
    , m_module(module) {
}

bool BaseNoiseNode::Update(std::string& error) noexcept {
    if (!m_isFull) {
        return true;
    }

    utils::NoiseMap noiseMap;
    utils::NoiseMapBuilderPlane noiseMapBuilder;
    noiseMapBuilder.SetSourceModule(*m_module);
    noiseMapBuilder.SetDestNoiseMap(noiseMap);
    noiseMapBuilder.SetDestSize(static_cast<int>(m_previewSize), static_cast<int>(m_previewSize));
    noiseMapBuilder.SetBounds(2.0, 6.0, 1.0, 5.0);
    noiseMapBuilder.Build();

    utils::Image image;
    utils::RendererImage renderer;
    renderer.SetSourceNoiseMap(noiseMap);
    renderer.SetDestImage(image);
    renderer.Render();

    auto header = m_imagePreview.view.header;
    bool needRecreate = ((header.height != m_previewSize) || (header.width != m_previewSize));
    if (needRecreate) {
        m_imagePreview.Create(ImageHeader(m_previewSize, m_previewSize, PixelFormat::R8G8B8A8));
    }

    size_t cntPixel = image.GetMemUsed();
    auto* inPtr = image.GetSlabPtr();
    auto* outPtr = static_cast<uint8_t*>(m_imagePreview.view.data);
    for (size_t i=0 ; i!=cntPixel; ++i) {
        const auto color = *inPtr++;
        *outPtr++ = color.red;
        *outPtr++ = color.green;
        *outPtr++ = color.blue;
        *outPtr++ = color.alpha;
    }

    if (!m_texturePreview || needRecreate) {
        m_texturePreview = TextureManager::Get().Create(m_imagePreview.view, error);
        if (!m_texturePreview) {
            return false;
        }
    } else {
        if (!m_texturePreview->Update(m_imagePreview.view, error)) {
            return false;
        }
    }

    return true;
}

bool BaseNoiseNode::DrawSettings() noexcept {
    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();

    ImGui::BeginGroup();
    ImGui::PushItemWidth(180);

    bool changed = DrawSettingsImpl();

    ImGui::PopItemWidth();
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

    return changed;
}

void BaseNoiseNode::DrawPreview() noexcept {
    if (m_isFull) {
        ImGui::SameLine();
        gui::Image(m_texturePreview, math::Size(m_previewSize, m_previewSize), math::Pointf(0,1), math::Pointf(1,0));
    }
}

BillowNode::BillowNode()
    : BaseNoiseNode(this, "Billow noise") {
    AddOutPin(new BasePin(0));
}

bool BillowNode::DrawSettingsImpl() noexcept {
    bool changed = false;

    changed |= gui::Combo("Quality", m_noiseQuality, QualityItems, noise::NoiseQuality(noise::NoiseQuality::QUALITY_BEST + 1));
    changed |= gui::InputScalar("Frequency", m_frequency, gui::Step(0.1, 1.0), "%.1f");
    changed |= gui::InputScalar("Lacunarity", m_lacunarity, gui::Step(0.01, 0.1), gui::Range(1.5, 3.5), "%.2f");
    changed |= gui::InputScalar("Octave count", m_octaveCount, gui::Step(1, 2), gui::Range(1, noise::module::BILLOW_MAX_OCTAVE));
    changed |= gui::InputScalar("Persistence", m_persistence, gui::Step(0.01, 0.1), gui::Range(0.0, 1.0), "%.2f");
    changed |= gui::InputScalar("Seed", m_seed, gui::Step(1, 1));

    return changed;
}

CheckerboardNode::CheckerboardNode()
    : BaseNoiseNode(this, "Checkerboard noise") {
    AddOutPin(new BasePin(0));
}

PerlinNode::PerlinNode()
    : BaseNoiseNode(this, "Perlin noise") {
    AddOutPin(new BasePin(0));
}

bool PerlinNode::DrawSettingsImpl() noexcept {
    bool changed = false;

    changed |= gui::Combo("Quality", m_noiseQuality, QualityItems, noise::NoiseQuality(noise::NoiseQuality::QUALITY_BEST + 1));
    changed |= gui::InputScalar("Frequency", m_frequency, gui::Step(0.1, 1.0), "%.1f");
    changed |= gui::InputScalar("Lacunarity", m_lacunarity, gui::Step(0.01, 0.1), gui::Range(1.5, 3.5), "%.2f");
    changed |= gui::InputScalar("Octave count", m_octaveCount, gui::Step(1, 2), gui::Range(1, noise::module::PERLIN_MAX_OCTAVE));
    changed |= gui::InputScalar("Persistence", m_persistence, gui::Step(0.01, 0.1), gui::Range(0.0, 1.0), "%.2f");
    changed |= gui::InputScalar("Seed", m_seed, gui::Step(1, 1));

    return changed;
}

RidgedMultiNode::RidgedMultiNode()
    : BaseNoiseNode(this, "RidgedMulti noise") {
    AddOutPin(new BasePin(0));
}

bool RidgedMultiNode::DrawSettingsImpl() noexcept {
    bool changed = false;

    changed |= gui::Combo("Quality", m_noiseQuality, QualityItems, noise::NoiseQuality(noise::NoiseQuality::QUALITY_BEST + 1));
    changed |= gui::InputScalar("Frequency", m_frequency, gui::Step(0.1, 1.0), "%.1f");
    changed |= gui::InputScalar("Lacunarity", m_lacunarity, gui::Step(0.01, 0.1), gui::Range(1.5, 3.5), "%.2f");
    changed |= gui::InputScalar("Octave count", m_octaveCount, gui::Step(1, 2), gui::Range(1, noise::module::RIDGED_MAX_OCTAVE));
    changed |= gui::InputScalar("Seed", m_seed, gui::Step(1, 1));

    return changed;
}

ScaleBiasNode::ScaleBiasNode()
    : BaseNoiseNode(this, "ScaleBias modifier") {

    m_isFull = false;
    AddInPin(new BasePin(0));
    AddOutPin(new BasePin(0));
}

bool ScaleBiasNode::OnIncomingLink(BasePin* src, BasePin* dst, bool checkOnly) noexcept {
    auto* srcNode = dynamic_cast<BaseNoiseNode*>(src->GetNode());
    if (!srcNode) {
        return false;
    }
    auto index = static_cast<int>(dst->GetUserIndex());
    if ((index >= GetSourceModuleCount()) || (index < 0)) {
        return false;
    }

    if (checkOnly) {
        return true;
    }

    SetSourceModule(index, srcNode->GetModule());

    try {
        for (int i=0; i!=GetSourceModuleCount(); ++i) {
                GetSourceModule(i);
        }
        m_isFull = true;
    } catch(const noise::ExceptionNoModule&) {
        m_isFull = false;
    }

    return true;
}

bool ScaleBiasNode::DrawSettingsImpl() noexcept {
    bool changed = false;

    changed |= gui::InputScalar("Bias", m_bias, gui::Step(0.1, 1.0), "%.1f");
    changed |= gui::InputScalar("Scale", m_scale, gui::Step(0.1, 1.0), "%.1f");

    return changed;
}
