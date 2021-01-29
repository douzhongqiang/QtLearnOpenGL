#ifndef STENCILTESTATTRNODE_H
#define STENCILTESTATTRNODE_H

#include "NDNodeBase.h"
#include "StencilTestRenderWidget.h"

class NDColorAttribute;
class NDRealAttribute;
class NDVec3Attribute;

class StencilTestAttrNode : public NDNodeBase
{
    Q_OBJECT

public:
    StencilTestAttrNode(QObject* parent = nullptr);
    ~StencilTestAttrNode();

    // 设置和获取光的信息
    void setLightInfo(const StencilTestRenderWidget::LightInfo& lightInfo);
    StencilTestRenderWidget::LightInfo getLightInfo(void);

    void setCameraPostion(const QVector3D& cameraPos);
    QVector3D getCameraPostion();

    void setCameraFront(const QVector3D& cameraFront);
    QVector3D getCameraFront(void);

private:
    void initAttribute(void);

    // 物体材质属性
    NDRealAttribute* m_pShininessAtr = nullptr;

    // 光的材质属性
    NDVec3Attribute* m_pLightPostionAttr = nullptr;
    NDColorAttribute* m_pLightAmbientColorAttr = nullptr;
    NDColorAttribute* m_pLightDiffuseColorAttr = nullptr;
    NDColorAttribute* m_pLightSpecularColorAttr = nullptr;

    // 相机的属性
    NDVec3Attribute* m_pCameraPosAttr = nullptr;
    NDVec3Attribute* m_pCameraFront = nullptr;

signals:
    void attributeValueChanged(const QVariant& color, bool cmd);
};

#endif
