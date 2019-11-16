#pragma once

#include <set>
#include <string>
#include <vector>

#include "engine/gui/math.h"
#include "engine/common/noncopyable.h"


enum class PinType : uint8_t {
    Noise3D,
    Noise2D,
    Image
};

class BaseNode;
class BasePin : Noncopyable {
    friend class BaseNode;
public:
    BasePin() = delete;
    BasePin(PinType pinType, uint32_t userIndex, math::Color color = math::Color(48, 220, 48));
    ~BasePin() = default;

    bool IsInput() const noexcept { return m_isInput; }
    PinType GetPinType() const noexcept { return m_pinType; }
    uint32_t GetUserIndex() const noexcept { return m_userIndex; }
    BaseNode* GetNode() const noexcept { return m_node; }

    void AddLink() noexcept { m_linkCount++; }
    void DelLink() noexcept;
    bool IsConnected() const noexcept { return m_linkCount > 0; }

    void Draw(uint8_t alpha) const;

protected:
    bool m_isInput = true;
    PinType m_pinType;
    uint32_t m_userIndex = 0;
    uint32_t m_linkCount = 0;
    math::Color m_color = math::Color(48, 220, 48);
    BaseNode* m_node = nullptr;
};

class BaseNode : Noncopyable {
protected:
    BaseNode() = delete;
    BaseNode(const std::string& name);
    virtual ~BaseNode();

public:
    // srcNode -> this (dstPin)
    virtual void SetSourceNode(BaseNode* srcNode, BasePin* dstPin);
    // srcNode -> this (dstPin)
    void DelSourceNode(BaseNode* srcNode, BasePin* dstPin);
    // this (srcPin) -> dstNode
    void AddDestNode(BaseNode* dstNode, BasePin* srcPin);
    // this (srcPin) -> dstNode
    void DelDestNode(BaseNode* dstNode, BasePin* srcPin);

    virtual void Update() = 0;
    void Draw();

protected:
    void AddInPin(BasePin* pin);
    void AddOutPin(BasePin* pin);

    void SetNeedUpdate() noexcept;

    void CheckIsFull() noexcept;
    void SetIsFull(bool value) noexcept;
    bool GetIsFull() const noexcept { return m_isFull; }

    virtual bool DrawSettings() = 0;
    virtual void DrawPreview() = 0;

private:
    std::string m_name;
    std::vector<BasePin*> m_inPins;
    std::vector<BasePin*> m_outPins;
    // this -> m_LinkedDstNodes
    std::set<BaseNode*> m_LinkedDstNodes;
    // m_LinkedSrcNodes -> this
    std::set<BaseNode*> m_LinkedSrcNodes;
    bool m_needUpdate = true;
    bool m_isFull = true;
    bool m_drawSettings = false;
};
