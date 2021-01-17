#ifndef LIGHTCASTERNODE_H
#define LIGHTCASTERNODE_H

#include "NDNodeBase.h"
#include "DirectionLightRenderWidget.h"
#include "PointLightRenderWidget.h"
#include "SpotLightRenderWidget.h"
#include "FlashLightRenderWidget.h"

class NDColorAttribute;
class NDRealAttribute;
class NDVec3Attribute;

class LightCasterNode : public NDNodeBase
{
    Q_OBJECT

public:
    LightCasterNode(QObject* parent = nullptr);
    ~LightCasterNode();

    // 平行光材质
    DirectionLightRenderWidget::LightInfo getLightInfo(void);
    void setLightInfo(const DirectionLightRenderWidget::LightInfo& lightInfo);
    // 点光源
    PointLightRenderWidget::LightInfo getPointLightInfo(void);
    void setPointLightInfo(const PointLightRenderWidget::LightInfo& lightInfo);
    // 聚光灯
    SpotLightRenderWidget::LightInfo getSpotLightInfo(void);
    void setSpotLightInfo(const SpotLightRenderWidget::LightInfo& lightInfo);
    // 手电筒
    FlashLightRenderWidget::LightInfo getFlashLightInfo(void);
    void setFlashLightInfo(const FlashLightRenderWidget::LightInfo& lightInfo);

private:
    void initAttribute(void);

    // 平行光的材质属性
    NDVec3Attribute* m_pLightPostionAttr = nullptr;
    NDColorAttribute* m_pLightAmbientColorAttr = nullptr;
    NDColorAttribute* m_pLightDiffuseColorAttr = nullptr;
    NDColorAttribute* m_pLightSpecularColorAttr = nullptr;
    void initDirectionLight(void);

    // 点光源的材质属性
    NDVec3Attribute* m_pPointLightPostionAttr = nullptr;
    NDColorAttribute* m_pPointLightAmbientColorAttr = nullptr;
    NDColorAttribute* m_pPointLightDiffuseColorAttr = nullptr;
    NDColorAttribute* m_pPointLightSpecularColorAttr = nullptr;
    void initPointLight(void);

    //  聚光灯
    NDVec3Attribute* m_pSpotLightPostionAttr = nullptr;
    NDVec3Attribute* m_pSpotLightDirectionAttr = nullptr;
    NDRealAttribute* m_pSpotLightInnerAttrValue = nullptr;
    NDRealAttribute* m_pSpotLightOuterAttrValue = nullptr;
    NDColorAttribute* m_pSpotLightAmbientColorAttr = nullptr;
    NDColorAttribute* m_pSpotLightDiffuseColorAttr = nullptr;
    NDColorAttribute* m_pSpotLightSpecularColorAttr = nullptr;
    void initSpotLight(void);

    //  手电筒
    NDVec3Attribute* m_pFlashLightPostionAttr = nullptr;
    NDVec3Attribute* m_pFlashLightDirectionAttr = nullptr;
    NDRealAttribute* m_pFlashLightInnerAttrValue = nullptr;
    NDRealAttribute* m_pFlashLightOuterAttrValue = nullptr;
    NDColorAttribute* m_pFlashLightAmbientColorAttr = nullptr;
    NDColorAttribute* m_pFlashLightDiffuseColorAttr = nullptr;
    NDColorAttribute* m_pFlashLightSpecularColorAttr = nullptr;
    void initFlashLight(void);

signals:
    void attributeValueChanged(const QVariant& color, bool cmd);
};

#endif
