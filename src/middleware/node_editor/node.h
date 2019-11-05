#pragma once

#include <set>
#include <memory>
#include <string>
#include <vector>

#include "engine/gui/math.h"
#include "engine/common/noncopyable.h"


enum class PinType : uint8_t {
    Noise,
    NoiseMap,
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

    void Draw(uint8_t alpha) const noexcept;

protected:
    bool m_isInput = true;
    PinType m_pinType;
    uint32_t m_userIndex = 0;
    uint32_t m_linkCount = 0;
    math::Color m_color = math::Color(48, 220, 48);
    BaseNode* m_node = nullptr;
};

class BaseNode : Noncopyable {
public:
    BaseNode() = delete;
    BaseNode(const std::string& name);
    virtual ~BaseNode();

    void AddInPin(BasePin* pin);
    void AddOutPin(BasePin* pin);

    // srcNode -> this (dstPin)
    bool SetSourceNode(BaseNode* srcNode, BasePin* dstPin, bool checkOnly) noexcept;
    // srcNode -> this (dstPin)
    void DelSourceNode(BaseNode* srcNode, BasePin* dstPin) noexcept;
    // this -> dstNode
    void AddDestNode(BaseNode* dstNode) noexcept;
    // this -> dstNode
    void DelDestNode(BaseNode* dstNode) noexcept;
    void SetNeedUpdate() noexcept;
    void CheckIsFull() noexcept;
    void SetIsFull(bool value) noexcept;
    bool GetIsFull() const noexcept { return m_isFull; }
    void Draw() noexcept;

    virtual bool OnSetSourceNode(BaseNode* srcNode, BasePin* dstPin, bool checkOnly) noexcept = 0;
    virtual void OnDelSourceNode(BaseNode* srcNode, BasePin* dstPin) noexcept = 0;
    virtual bool Update(std::string& error) noexcept = 0;
    virtual bool CheckIsConsistency() noexcept = 0;
    virtual bool OnDrawSettings() noexcept = 0;
    virtual void OnDrawPreview() noexcept = 0;

private:
    std::string m_name;
    std::vector<BasePin*> m_inPins;
    std::vector<BasePin*> m_outPins;
    // this -> m_LinkedDstNodes
    std::set<BaseNode*> m_LinkedDstNodes;
    // m_LinkedSrcNodes -> this
    std::set<BaseNode*> m_LinkedSrcNodes;
    bool m_wrongNode = false;
    bool m_needUpdate = true;
    bool m_isFull = true;
    bool m_drawSettings = false;
};
