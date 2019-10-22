#pragma once

#include <memory>
#include <string>
#include <vector>

#include "engine/common/noncopyable.h"


class BasePin : Noncopyable {
    friend class BaseNode;
public:
    BasePin() = default;
    ~BasePin() = default;

    void Draw(bool connected, uint8_t alpha) const noexcept;
protected:
    bool m_isInput = true;
};

class BaseNode : Noncopyable {
public:
    BaseNode() = delete;
    BaseNode(const std::string& name);
    virtual ~BaseNode();

    void AddInPin(BasePin* item);
    void AddOutPin(BasePin* item);
    void Draw() noexcept;
    virtual bool Update(std::string& /*error*/) noexcept { return true; }
    virtual bool DrawSettings() noexcept { return false; }
    virtual void DrawPreview() noexcept {}

protected:
    uint32_t m_previewSize = 64;

private:
    std::string m_name;
    std::vector<BasePin*> m_inPins;
    std::vector<BasePin*> m_outPins;
    bool m_wrongNode = false;
    bool m_needUpdate = true;
    bool m_drawSettings = false;
};