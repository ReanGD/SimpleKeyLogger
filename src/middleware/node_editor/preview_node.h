#pragma once

#include <memory>

#include "engine/material/image.h"
#include "middleware/node_editor/node.h"


namespace noise {
    namespace utils {
        class Shape;
        class RendererImage;
    }
    namespace module {
        class Module;
    }
}
class Texture;
class PreviewNode : public BaseNode {
protected:
    PreviewNode(const std::string& name);
    ~PreviewNode() override;

    void UpdatePreview(const ImageView& view);
    void UpdatePreview(const noise::utils::Shape* sourceModule);
    void UpdatePreview(const noise::module::Module* sourceModule);

    void DrawPreview() final;

private:
    uint32_t m_texSize = 128;
    uint32_t m_previewSize = 128;
    std::shared_ptr<Texture> m_texturePreview = nullptr;
    noise::utils::RendererImage* m_renderedPreview = nullptr;
    noise::utils::Shape* m_shapePreview = nullptr;
};
