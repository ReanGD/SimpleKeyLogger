#pragma once

#include <memory>

#include "middleware/node_editor/node.h"
#include "engine/material/texture_manager.h"


namespace noise {
    namespace utils {
        class RendererImage;
    }
}
class BaseNoise2DNode;
class BaseNoise3DNode;
class PreviewNode : public BaseNode {
protected:
    PreviewNode(const std::string& name);
    ~PreviewNode() override;

    void UpdatePreview(const ImageView& view);
    void UpdatePreview(const BaseNoise2DNode* sourceModule);
    void UpdatePreview(BaseNoise3DNode* sourceModule);

    void DrawPreview() final;

public:
    std::shared_ptr<Texture> GetView();

private:
    uint32_t m_previewSize = 128;
    DynamicTexture m_texturePreview;
    noise::utils::RendererImage* m_renderedPreview = nullptr;
    BaseNoise2DNode* m_shapePreview = nullptr;
};
