#pragma once

#include <noise.h>

#include "middleware/node_editor/preview_node.h"


class BaseNoise2DNode : public PreviewNode {
protected:
    BaseNoise2DNode(const std::string& name);

public:
    void Update() override;
    virtual double GetValue(double u, double v) const = 0;

protected:
    bool DrawSettings() override { return false; }
    virtual bool OnDrawSettings() { return false; }
};

class PlaneNode: public BaseNoise2DNode {
public:
    PlaneNode();

    double GetValue(double u, double v) const override;
    void SetSourceNode(BaseNode* srcNode, BasePin* dstPin) override;

private:
    noise::model::Plane m_model;
};

class SphereNode: public BaseNoise2DNode {
public:
    SphereNode();

    double GetValue(double u, double v) const override;
    void SetSourceNode(BaseNode* srcNode, BasePin* dstPin) override;

private:
    noise::model::Sphere m_model;
};

class CylinderNode: public BaseNoise2DNode {
public:
    CylinderNode();

    double GetValue(double u, double v) const override;
    void SetSourceNode(BaseNode* srcNode, BasePin* dstPin) override;

private:
    noise::model::Cylinder m_model;
};
