#include "middleware/node_editor/noise_node.h"

#include <imgui.h>
#include "engine/gui/widgets.h"


static const char* QualityItems[] = {"Fast", "Std", "Best"};

BillowNode::BillowNode()
    : BaseNoiseNode(this, "Billow") {
    AddOutPin(new BasePin(PinType::Noise, 0));
}

bool BillowNode::OnDrawSettingsImpl() {
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
    : BaseNoiseNode(this, "Checkerboard") {
    AddOutPin(new BasePin(PinType::Noise, 0));
}

ConstNode::ConstNode()
    : BaseNoiseNode(this, "Const") {
    AddOutPin(new BasePin(PinType::Noise, 0));
}

bool ConstNode::OnDrawSettingsImpl() {
    bool changed = false;

    changed |= gui::InputScalar("Value", m_constValue, gui::Step(0.01, 0.1), gui::Range(-1.001, 1.001), "%.2f");

    return changed;
}

CylindersNode::CylindersNode()
    : BaseNoiseNode(this, "Cylinders") {
    AddOutPin(new BasePin(PinType::Noise, 0));
}

bool CylindersNode::OnDrawSettingsImpl() {
    bool changed = false;

    changed |= gui::InputScalar("Frequency", m_frequency, gui::Step(0.1, 1.0), "%.1f");

    return changed;
}

PerlinNode::PerlinNode()
    : BaseNoiseNode(this, "Perlin") {
    AddOutPin(new BasePin(PinType::Noise, 0));
}

bool PerlinNode::OnDrawSettingsImpl() {
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
    : BaseNoiseNode(this, "RidgedMulti") {
    AddOutPin(new BasePin(PinType::Noise, 0));
}

bool RidgedMultiNode::OnDrawSettingsImpl() {
    bool changed = false;

    changed |= gui::Combo("Quality", m_noiseQuality, QualityItems, noise::NoiseQuality(noise::NoiseQuality::QUALITY_BEST + 1));
    changed |= gui::InputScalar("Frequency", m_frequency, gui::Step(0.1, 1.0), "%.1f");
    if (gui::InputScalar("Lacunarity", m_lacunarity, gui::Step(0.01, 0.1), gui::Range(1.5, 3.5), "%.2f")) {
        SetLacunarity(m_lacunarity);
        changed = true;
    }
    changed |= gui::InputScalar("Octave count", m_octaveCount, gui::Step(1, 2), gui::Range(1, noise::module::RIDGED_MAX_OCTAVE));
    changed |= gui::InputScalar("Seed", m_seed, gui::Step(1, 1));

    return changed;
}

SpheresNode::SpheresNode()
    : BaseNoiseNode(this, "Spheres") {
    AddOutPin(new BasePin(PinType::Noise, 0));
}

bool SpheresNode::OnDrawSettingsImpl() {
    bool changed = false;

    changed |= gui::InputScalar("Frequency", m_frequency, gui::Step(0.1, 1.0), "%.1f");

    return changed;
}

VoronoiNode::VoronoiNode()
    : BaseNoiseNode(this, "Voronoi") {
    AddOutPin(new BasePin(PinType::Noise, 0));
}

bool VoronoiNode::OnDrawSettingsImpl() {
    bool changed = false;

    changed |= gui::InputScalar("Displacement", m_displacement, gui::Step(0.1, 1.0), "%.1f");
    changed |= gui::Checkbox("Enable distance", m_enableDistance);
    changed |= gui::InputScalar("Frequency", m_frequency, gui::Step(0.1, 1.0), "%.1f");
    changed |= gui::InputScalar("Seed", m_seed, gui::Step(1, 1));

    return changed;
}
