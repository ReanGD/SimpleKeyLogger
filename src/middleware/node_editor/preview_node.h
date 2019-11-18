#pragma once

#include <memory>

#include "engine/material/image.h"
#include "middleware/node_editor/node.h"


namespace noise {
    namespace utils {
        class RendererImage;
    }
}
class Texture;
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

private:
    uint32_t m_texSize = 128;
    uint32_t m_previewSize = 128;
    std::shared_ptr<Texture> m_texturePreview = nullptr;
    noise::utils::RendererImage* m_renderedPreview = nullptr;
    BaseNoise2DNode* m_shapePreview = nullptr;
};
