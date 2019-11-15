#include "middleware/node_editor/noise_3d.h"

#include "engine/gui/widgets.h"


static const char* QualityItems[] = {"Fast", "Std", "Best"};

BillowNode::BillowNode()
    : BaseNoise3DNode(this, "Billow") {
}

bool BillowNode::OnDrawSettings() {
    bool changed = false;

    changed |= gui::SliderEnum("Quality", m_noiseQuality, QualityItems, noise::NoiseQuality(noise::NoiseQuality::QUALITY_BEST + 1));
    changed |= gui::InputScalar("Frequency", m_frequency, gui::Step(0.1, 1.0), "%.1f");
    changed |= gui::InputScalar("Lacunarity", m_lacunarity, gui::Step(0.01, 0.1), gui::Range(1.5, 3.5), "%.2f");
    changed |= gui::InputScalar("Octave count", m_octaveCount, gui::Step(1, 2), gui::Range(1, noise::module::BILLOW_MAX_OCTAVE));
    changed |= gui::InputScalar("Persistence", m_persistence, gui::Step(0.01, 0.1), gui::Range(0.0, 1.0), "%.2f");
    changed |= gui::InputScalar("Seed", m_seed, gui::Step(1, 1));

    return changed;
}

CheckerboardNode::CheckerboardNode()
    : BaseNoise3DNode(this, "Checkerboard") {
}

ConstNode::ConstNode()
    : BaseNoise3DNode(this, "Const") {
}

bool ConstNode::OnDrawSettings() {
    bool changed = false;

    changed |= gui::InputScalar("Value", m_constValue, gui::Step(0.01, 0.1), gui::Range(-1.001, 1.001), "%.2f");

    return changed;
}

CylindersNode::CylindersNode()
    : BaseNoise3DNode(this, "Cylinders") {
}

bool CylindersNode::OnDrawSettings() {
    bool changed = false;

    changed |= gui::InputScalar("Frequency", m_frequency, gui::Step(0.1, 1.0), "%.1f");

    return changed;
}

PerlinNode::PerlinNode()
    : BaseNoise3DNode(this, "Perlin") {
}

bool PerlinNode::OnDrawSettings() {
    bool changed = false;

    changed |= gui::SliderEnum("Quality", m_noiseQuality, QualityItems, noise::NoiseQuality(noise::NoiseQuality::QUALITY_BEST + 1));
    changed |= gui::InputScalar("Frequency", m_frequency, gui::Step(0.1, 1.0), "%.1f");
    changed |= gui::InputScalar("Lacunarity", m_lacunarity, gui::Step(0.01, 0.1), gui::Range(1.5, 3.5), "%.2f");
    changed |= gui::InputScalar("Octave count", m_octaveCount, gui::Step(1, 2), gui::Range(1, noise::module::PERLIN_MAX_OCTAVE));
    changed |= gui::InputScalar("Persistence", m_persistence, gui::Step(0.01, 0.1), gui::Range(0.0, 1.0), "%.2f");
    changed |= gui::InputScalar("Seed", m_seed, gui::Step(1, 1));

    return changed;
}

RidgedMultiNode::RidgedMultiNode()
    : BaseNoise3DNode(this, "RidgedMulti") {
}

bool RidgedMultiNode::OnDrawSettings() {
    bool changed = false;

    changed |= gui::SliderEnum("Quality", m_noiseQuality, QualityItems, noise::NoiseQuality(noise::NoiseQuality::QUALITY_BEST + 1));
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
    : BaseNoise3DNode(this, "Spheres") {
}

bool SpheresNode::OnDrawSettings() {
    bool changed = false;

    changed |= gui::InputScalar("Frequency", m_frequency, gui::Step(0.1, 1.0), "%.1f");

    return changed;
}

VoronoiNode::VoronoiNode()
    : BaseNoise3DNode(this, "Voronoi") {
}

bool VoronoiNode::OnDrawSettings() {
    bool changed = false;

    changed |= gui::InputScalar("Displacement", m_displacement, gui::Step(0.1, 1.0), "%.1f");
    changed |= gui::Checkbox("Enable distance", m_enableDistance);
    changed |= gui::InputScalar("Frequency", m_frequency, gui::Step(0.1, 1.0), "%.1f");
    changed |= gui::InputScalar("Seed", m_seed, gui::Step(1, 1));

    return changed;
}
