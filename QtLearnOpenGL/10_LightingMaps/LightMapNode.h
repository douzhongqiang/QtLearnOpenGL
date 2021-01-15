#ifndef LIGHTMAPNODE_H
#define LIGHTMAPNODE_H

#include "NDNodeBase.h"
#include "LightMapRenderWidget.h"

class NDColorAttribute;
class NDRealAttribute;
class NDVec3Attribute;

class LightMapNode : public NDNodeBase
{
    Q_OBJECT

public:
    LightMapNode(QObject* parent = nullptr);
    ~LightMapNode();

    void setObjectMaterial(const LightMapRenderWidget::ObjectMaterial& objectMaterial);
    void setLightInfo(const LightMapRenderWidget::LightInfo& lightInfo);

    LightMapRenderWidget::ObjectMaterial getObjectMaterial(void);
    LightMapRenderWidget::LightInfo getLightInfo(void);

private:
    void initAttribute(void);

    // 物体材质属性
//    NDColorAttribute* m_pAmbientColorAttr = nullptr;
//    NDColorAttribute* m_pDiffuseColorAttr = nullptr;
//    NDColorAttribute* m_pSpecularColorAttr = nullptr;
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
