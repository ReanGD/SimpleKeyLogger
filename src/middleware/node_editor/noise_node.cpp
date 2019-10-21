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


PerlinNode::PerlinNode()
    : BaseNode("Perlin noise") {
        AddOutPin(new BasePin());
}

bool PerlinNode::Create(std::string& error) noexcept {
    auto image = GenerateImage();
    m_preview = TextureManager::Get().Create(image, error);
    uint8* texData = reinterpret_cast<uint8*>(image.data);
    delete []texData;
    if (!m_preview) {
        return false;
    }

    return true;
}

bool PerlinNode::DrawSettings() {
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

void PerlinNode::DrawPreview() {
    ImGui::SameLine();
    gui::Image(m_preview, math::Size(m_previewSize, m_previewSize), math::Pointf(0,1), math::Pointf(1,0));
}

ImageView PerlinNode::GenerateImage() const noexcept {
    module::Perlin module;
    module.SetFrequency(m_frequency);
    module.SetLacunarity(m_lacunarity);
    module.SetNoiseQuality(ToNoiseType(m_noiseQuality));
    module.SetOctaveCount(m_octaveCount);
    module.SetPersistence(m_persistence);
    module.SetSeed(m_seed);

    utils::NoiseMap noiseMap;
    utils::NoiseMapBuilderPlane noiseMapBuilder;
    noiseMapBuilder.SetSourceModule(module);
    noiseMapBuilder.SetDestNoiseMap(noiseMap);
    noiseMapBuilder.SetDestSize(static_cast<int>(m_previewSize), static_cast<int>(m_previewSize));
    noiseMapBuilder.SetBounds(2.0, 6.0, 1.0, 5.0);
    noiseMapBuilder.Build();

    utils::Image image;
    utils::RendererImage renderer;
    renderer.SetSourceNoiseMap(noiseMap);
    renderer.SetDestImage(image);
    renderer.Render();

    size_t cntPixel = image.GetMemUsed();
    utils::Color* colors = image.GetSlabPtr();
    size_t outBytePerPixel = 4;
    uint8* texData = new uint8[cntPixel * outBytePerPixel];
    for (size_t i=0 ; i!=cntPixel; ++i) {
        utils::Color color = colors[i];
        texData[i * outBytePerPixel + 0] = color.red;
        texData[i * outBytePerPixel + 1] = color.green;
        texData[i * outBytePerPixel + 2] = color.blue;
        texData[i * outBytePerPixel + 3] = color.alpha;
    }

    ImageHeader header(m_previewSize, m_previewSize, PixelFormat::R8G8B8A8);
    return ImageView(header, 1, texData);
}
