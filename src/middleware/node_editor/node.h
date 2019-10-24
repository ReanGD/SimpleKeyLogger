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
    BasePin(uint32_t userIndex);
    ~BasePin() = default;

    bool IsInput() const noexcept { return m_isInput; }
    uint32_t GetUserIndex() const noexcept { return m_userIndex; }
    BaseNode* GetNode() const noexcept { return m_node; }
    bool IsConnected() const noexcept { return m_linkCount > 0; }
    void AddLink() noexcept { m_linkCount++; }

    void Draw(uint8_t alpha) const noexcept;
protected:
    bool m_isInput = true;
    uint32_t m_userIndex = 0;
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

    bool AddIncomingLink(BasePin* src, BasePin* dst, bool checkOnly);
    void Draw() noexcept;

    virtual bool OnIncomingLink(BasePin* /*src*/, BasePin* /*dst*/, bool /*checkOnly*/) noexcept { return false; }
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
