#ifndef CUBEMAPSATTRNODE_H
#define CUBEMAPSATTRNODE_H

#include "NDNodeBase.h"
#include "CubemapsRenderWidget.h"

class NDColorAttribute;
class NDRealAttribute;
class NDVec3Attribute;
class NDBoolAttribute;
class NDString2Attribute;

class CubemapsAttrNode : public NDNodeBase
{
    Q_OBJECT

public:
    CubemapsAttrNode(QObject* parent = nullptr);
    ~CubemapsAttrNode();

    // 设置和获取光的信息
    void setLightInfo(const CubemapsRenderWidget::LightInfo& lightInfo);
    CubemapsRenderWidget::LightInfo getLightInfo(void);

    void setCameraPostion(const QVector3D& cameraPos);
    QVector3D getCameraPostion();

    void setCameraFront(const QVector3D& cameraFront);
    QVector3D getCameraFront(void);

    // 面剔除相关
    void setCullFaceEnabled(bool isEnabled);
    bool isCullFaceEnabled(void);

    // 设置要剔除的面
    void setCullFaceType(CubemapsRenderWidget::CullFaceType type);
    CubemapsRenderWidget::CullFaceType getCullFaceType(void);

    // 设置正面的顺序
    void setFrontOrderType(CubemapsRenderWidget::FrontFaceOrderType type);
    CubemapsRenderWidget::FrontFaceOrderType getFrontOrderType(void);

    // 设置/获取当前的后期处理效果
    void setCurrentPostProcessType(CubemapsRenderWidget::PostProcessType type);
    CubemapsRenderWidget::PostProcessType getCurrentPostProcessType(void);

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

    // 后期处理属性
    NDString2Attribute* m_pPostProcessTypeAttr = nullptr;

signals:
    void attributeValueChanged(const QVariant& color, bool cmd);
};

#endif
