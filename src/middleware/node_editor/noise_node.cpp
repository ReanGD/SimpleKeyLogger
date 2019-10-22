#include "middleware/node_editor/noise_node.h"

#include <noise.h>
#include <noise/noiseutils.h>
#include <imgui_node_editor.h>
#include <imgui_internal.h>

#include "engine/gui/widgets.h"
#include "engine/material/texture_manager.h"


namespace ne = ax::NodeEditor;

static const char* QualityItems[] = {"Fast", "Std", "Best"};
static noise::NoiseQuality ToNoiseType(const PerlinNode::Quality value) noexcept {
    switch (value) {
    case PerlinNode::Quality::Fast: return noise::NoiseQuality::QUALITY_FAST;
    case PerlinNode::Quality::Std: return noise::NoiseQuality::QUALITY_STD;
    case PerlinNode::Quality::Best: return noise::NoiseQuality::QUALITY_BEST;
    default: return noise::NoiseQuality::QUALITY_STD;
    }
}

BaseNoiseNode::BaseNoiseNode(const std::string& name)
    : BaseNode(name) {
}

bool BaseNoiseNode::UpdateImpl(noise::module::Module* module, std::string& error) noexcept {
    utils::NoiseMap noiseMap;
    utils::NoiseMapBuilderPlane noiseMapBuilder;
    noiseMapBuilder.SetSourceModule(*module);
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

void BaseNoiseNode::DrawPreview() noexcept {
    ImGui::SameLine();
    gui::Image(m_texturePreview, math::Size(m_previewSize, m_previewSize), math::Pointf(0,1), math::Pointf(1,0));
}

PerlinNode::PerlinNode()
    : BaseNoiseNode("Perlin noise") {
        AddOutPin(new BasePin());
}

bool PerlinNode::Update(std::string& error) noexcept {
    module::Perlin module;
    module.SetFrequency(m_frequency);
    module.SetLacunarity(m_lacunarity);
    module.SetNoiseQuality(ToNoiseType(m_noiseQuality));
    module.SetOctaveCount(m_octaveCount);
    module.SetPersistence(m_persistence);
    module.SetSeed(m_seed);

    return UpdateImpl(&module, error);
}

bool PerlinNode::DrawSettings() noexcept {
    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();

    ImGui::BeginGroup();
    ImGui::PushItemWidth(180);

    bool changed = false;

    changed |= gui::Combo("Quality", m_noiseQuality, QualityItems, Quality::Count);
    changed |= gui::InputScalar("Frequency", m_frequency, gui::Step(0.1, 1.0), "%.1f");
    changed |= gui::InputScalar("Lacunarity", m_lacunarity, gui::Step(0.01, 0.1), gui::Range(1.5, 3.5), "%.2f");
    changed |= gui::InputScalar("Octave count", m_octaveCount,
        gui::Step(uint8_t(1), uint8_t(2)),
        gui::Range(uint8_t(1), static_cast<uint8_t>(noise::module::PERLIN_MAX_OCTAVE)));
    changed |= gui::InputScalar("Persistence", m_persistence, gui::Step(0.01, 0.1), gui::Range(0.0, 1.0), "%.2f");
    changed |= gui::InputScalar("Seed", m_seed, gui::Step(1, 1));

    ImGui::PopItemWidth();
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

    return changed;
}

BillowNode::BillowNode()
    : BaseNoiseNode("Billow noise") {
        AddOutPin(new BasePin());
}

bool BillowNode::Update(std::string& error) noexcept {
    module::Billow module;
    module.SetFrequency(m_frequency);
    module.SetLacunarity(m_lacunarity);
    module.SetNoiseQuality(ToNoiseType(m_noiseQuality));
    module.SetOctaveCount(m_octaveCount);
    module.SetPersistence(m_persistence);
    module.SetSeed(m_seed);

    return UpdateImpl(&module, error);
}

bool BillowNode::DrawSettings() noexcept {
    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();

    ImGui::BeginGroup();
    ImGui::PushItemWidth(180);

    bool changed = false;

    changed |= gui::Combo("Quality", m_noiseQuality, QualityItems, PerlinNode::Quality::Count);
    changed |= gui::InputScalar("Frequency", m_frequency, gui::Step(0.1, 1.0), "%.1f");
    changed |= gui::InputScalar("Lacunarity", m_lacunarity, gui::Step(0.01, 0.1), gui::Range(1.5, 3.5), "%.2f");
    changed |= gui::InputScalar("Octave count", m_octaveCount,
        gui::Step(uint8_t(1), uint8_t(2)),
        gui::Range(uint8_t(1), static_cast<uint8_t>(noise::module::BILLOW_MAX_OCTAVE)));
    changed |= gui::InputScalar("Persistence", m_persistence, gui::Step(0.01, 0.1), gui::Range(0.0, 1.0), "%.2f");
    changed |= gui::InputScalar("Seed", m_seed, gui::Step(1, 1));

    ImGui::PopItemWidth();
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

    return changed;
}
