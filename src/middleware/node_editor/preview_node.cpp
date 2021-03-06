#include "middleware/node_editor/preview_node.h"

#include <imgui.h>

#include "engine/gui/widgets.h"
#include "engine/common/exception.h"
#include "engine/material/texture_manager.h"
#include "middleware/node_editor/noise_2d.h"
#include "middleware/node_editor/noise_3d.h"
#include "middleware/node_editor/noiseutils.h"


PreviewNode::PreviewNode(const std::string& name)
    : BaseNode(name) {

}

PreviewNode::~PreviewNode() {
    if (m_shapePreview != nullptr) {
        delete m_shapePreview;
        m_shapePreview = nullptr;
    }
    if (m_renderedPreview != nullptr) {
        delete m_renderedPreview;
        m_renderedPreview = nullptr;
    }
}

void PreviewNode::UpdatePreview(const ImageView& view) {
    if (view.header.width != m_previewSize) {
        throw EngineError("the width={} of the image for preview should be equal previewSize={}", view.header.width, m_previewSize);
    }

    if (view.header.height != m_previewSize) {
        throw EngineError("the height={} of the image for preview should be equal previewSize={}", view.header.height, m_previewSize);
    }

    m_texturePreview.UpdateOrCreate(view);
}

void PreviewNode::UpdatePreview(const BaseNoise2DNode* sourceModule) {
    if (m_renderedPreview == nullptr) {
        m_renderedPreview = new noise::utils::RendererImage();
        m_renderedPreview->SetSourceModule(sourceModule);
        m_renderedPreview->SetDestSize(m_previewSize, m_previewSize);
        m_renderedPreview->SetBounds(2.0, 6.0, 1.0, 5.0);
    }

    UpdatePreview(m_renderedPreview->Render());
}

void PreviewNode::UpdatePreview(BaseNoise3DNode* sourceModule) {
    if (m_shapePreview == nullptr) {
        m_shapePreview = new PlaneNode();
        auto dstPin = BasePin(PinType::Noise3D, 0);
        m_shapePreview->SetSourceNode(sourceModule, &dstPin);
    }

    UpdatePreview(m_shapePreview);
}

void PreviewNode::DrawPreview() {
    ImGui::SameLine();
    gui::Image(m_texturePreview.GetTexture(), math::Size(m_previewSize, m_previewSize), math::Pointf(0,1), math::Pointf(1,0));
}

std::shared_ptr<Texture> PreviewNode::GetView() {
    return m_texturePreview.GetTexture();
}
