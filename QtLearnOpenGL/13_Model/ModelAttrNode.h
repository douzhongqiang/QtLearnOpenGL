#ifndef MODELATTRNODE_H
#define MODELATTRNODE_H

#include "NDNodeBase.h"
#include "ModelRenderWidget.h"

class NDColorAttribute;
class NDRealAttribute;
class NDVec3Attribute;

class ModelAttrNode : public NDNodeBase
{
    Q_OBJECT

public:
    ModelAttrNode(QObject* parent = nullptr);
    ~ModelAttrNode();

    // 设置和获取光的信息
    void setLightInfo(const ModelRenderWidget::LightInfo& lightInfo);
    ModelRenderWidget::LightInfo getLightInfo(void);

private:
    void initAttribute(void);

    // 物体材质属性
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
