#ifndef MATERIALNODE_H
#define MATERIALNODE_H

#include "NDNodeBase.h"
#include "MaterialRenderWidget.h"

class NDColorAttribute;
class NDRealAttribute;
class NDVec3Attribute;

class MaterialNode : public NDNodeBase
{
    Q_OBJECT

public:
    MaterialNode(QObject* parent = nullptr);
    ~MaterialNode();

    void setObjectMaterial(const MaterialRenderWidget::ObjectMaterial& objectMaterial);
    void setLightInfo(const MaterialRenderWidget::LightInfo& lightInfo);

    MaterialRenderWidget::ObjectMaterial getObjectMaterial(void);
    MaterialRenderWidget::LightInfo getLightInfo(void);

private:
    void initAttribute(void);

    // 物体材质属性
    NDColorAttribute* m_pAmbientColorAttr = nullptr;
    NDColorAttribute* m_pDiffuseColorAttr = nullptr;
    NDColorAttribute* m_pSpecularColorAttr = nullptr;
    NDRealAttribute* m_pShininessAtr = nullptr;

    // 光的材质属性
    NDVec3Attribute* m_pLightPostionAttr = nullptr;
    NDColorAttribute* m_pLightAmbientColorAttr = nullptr;
    NDColorAttribute* m_pLightDiffuseColorAttr = nullptr;
    NDColorAttribute* m_pLightSpecularColorAttr = nullptr;

signals:
    void attributeValueChanged(const QVariant& color, bool cmd);
};

#endif
