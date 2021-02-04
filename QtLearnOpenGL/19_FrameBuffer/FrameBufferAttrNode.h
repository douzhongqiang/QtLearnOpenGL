#ifndef FRAMEBUFFERATTRNODE_H
#define FRAMEBUFFERATTRNODE_H

#include "NDNodeBase.h"
#include "FrameBufferRenderWidget.h"

class NDColorAttribute;
class NDRealAttribute;
class NDVec3Attribute;
class NDBoolAttribute;
class NDString2Attribute;

class FrameBufferAttrNode : public NDNodeBase
{
    Q_OBJECT

public:
    FrameBufferAttrNode(QObject* parent = nullptr);
    ~FrameBufferAttrNode();

    // 设置和获取光的信息
    void setLightInfo(const FrameBufferRenderWidget::LightInfo& lightInfo);
    FrameBufferRenderWidget::LightInfo getLightInfo(void);

    void setCameraPostion(const QVector3D& cameraPos);
    QVector3D getCameraPostion();

    void setCameraFront(const QVector3D& cameraFront);
    QVector3D getCameraFront(void);

    // 面剔除相关
    void setCullFaceEnabled(bool isEnabled);
    bool isCullFaceEnabled(void);

    // 设置要剔除的面
    void setCullFaceType(FrameBufferRenderWidget::CullFaceType type);
    FrameBufferRenderWidget::CullFaceType getCullFaceType(void);

    // 设置正面的顺序
    void setFrontOrderType(FrameBufferRenderWidget::FrontFaceOrderType type);
    FrameBufferRenderWidget::FrontFaceOrderType getFrontOrderType(void);

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

    // 背面剔除属性
    NDBoolAttribute* m_pCullFaceEnabledAttr = nullptr;
    NDString2Attribute* m_pCullFaceTypeAttr = nullptr;
    NDString2Attribute* m_pFrontFaceTypeAttr = nullptr;
    NDAttributeGroup* getCullFaceAttrGroup(void);

signals:
    void attributeValueChanged(const QVariant& color, bool cmd);
};

#endif
