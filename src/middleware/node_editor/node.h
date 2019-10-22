#pragma once

#include <memory>
#include <string>
#include <vector>

#include "engine/common/noncopyable.h"


class BaseNode;
class BasePin : Noncopyable {
    friend class BaseNode;
public:
    BasePin() = delete;
    BasePin(uint32_t id);
    ~BasePin() = default;

    bool IsInput() const noexcept { return m_isInput; }
    uint32_t GetId() const noexcept { return m_id; }
    BaseNode* GetNode() const noexcept { return m_node; }
    void AddLink() noexcept { m_linkCount++; }

    void Draw(uint8_t alpha) const noexcept;
protected:
    bool m_isInput = true;
    uint32_t m_id = 0;
    uint32_t m_linkCount = 0;
    BaseNode* m_node = nullptr;
};

class BaseNode : Noncopyable {
public:
    BaseNode() = delete;
    BaseNode(const std::string& name);
    virtual ~BaseNode();

    void AddInPin(BasePin* pin);
    void AddOutPin(BasePin* pin);

    void AddIncomingLink(BasePin* src, BasePin* dst);
    void Draw() noexcept;

    virtual void OnIncomingLink(BasePin* /*src*/, BasePin* /*dst*/) noexcept { }
    virtual bool Update(std::string& /*error*/) noexcept = 0;
    virtual bool DrawSettings() noexcept = 0;
    virtual void DrawPreview() noexcept = 0;

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
