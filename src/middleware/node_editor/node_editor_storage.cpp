#include "middleware/node_editor/node_editor_storage.h"


namespace ne = ax::NodeEditor;

NodeEditorStorage::~NodeEditorStorage() {
    m_nodes.clear();
    m_links.clear();
}

void NodeEditorStorage::AddNode(const std::shared_ptr<BaseNode>& node) {
    m_nodes.push_back(node);
}

bool NodeEditorStorage::AddLink(const ne::PinId pinIdFirst, const ne::PinId pinIdSecond, bool checkOnly) {
    if ((!pinIdFirst) || (!pinIdSecond) || (pinIdFirst == pinIdSecond)) {
        return false;
    }

    BasePin* pinFirst = pinIdFirst.AsPointer<BasePin>();
    BasePin* pinSecond = pinIdSecond.AsPointer<BasePin>();

    if ((pinFirst->IsInput() == pinSecond->IsInput()) ||
        (pinFirst->GetNode() == pinSecond->GetNode()) ||
        (pinFirst->GetNode() == nullptr) ||
        (pinSecond->GetNode() == nullptr) ||
        (pinFirst->GetPinType() != pinSecond->GetPinType())) {
        return false;
    }

    auto* srcPin = pinFirst->IsInput() ? pinSecond : pinFirst;
    auto* srcNode = srcPin->GetNode();

    auto* dstPin = pinFirst->IsInput() ? pinFirst : pinSecond;
    auto* dstNode = dstPin->GetNode();
    if (dstPin->IsConnected()) {
        return false;
    }

    if (checkOnly && !dstNode->SetSourceNode(srcNode, dstPin, checkOnly)) {
        return false;
    }

    if (!checkOnly) {
        dstNode->SetSourceNode(srcNode, dstPin, checkOnly);
        srcNode->AddDestNode(dstNode);
        srcPin->AddLink();
        dstPin->AddLink();

        auto linkId = ne::LinkId(static_cast<uintptr_t>(m_nextId++));
        m_links[linkId] = LinkInfo{ne::PinId(srcPin), ne::PinId(dstPin)};
        ne::Link(linkId, ne::PinId(srcPin), ne::PinId(dstPin));
    }

    return true;
}

bool NodeEditorStorage::DelLink(const ne::LinkId linkId, bool checkOnly) {
    const auto it = m_links.find(linkId);
    if (it == m_links.cend()) {
        return false;
    }

    if (!checkOnly) {
        auto* src = it->second.srcPin.AsPointer<BasePin>();
        auto* srcNode = src->GetNode();
        src->DelLink();

        auto* dst = it->second.dstPin.AsPointer<BasePin>();
        auto* dstNode = dst->GetNode();
        dst->DelLink();

        dstNode->DelSourceNode(srcNode, dst);
        srcNode->DelDestNode(dstNode);

        m_links.erase(it);
    }

    return true;
}

void NodeEditorStorage::Draw() {
    for (const auto& node: m_nodes) {
        node->Draw();
    }

    for (const auto& [linkId, info] : m_links) {
        ne::Link(linkId, info.srcPin, info.dstPin);
    }
}
