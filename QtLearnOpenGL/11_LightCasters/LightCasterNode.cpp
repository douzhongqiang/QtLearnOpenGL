#include "LightCasterNode.h"
#include "NDColorAttribute.h"
#include "NDRealAttribute.h"
#include "NDAttributeGroup.h"
#include "NDVec3Attribute.h"
#include "Utils.h"

LightCasterNode::LightCasterNode(QObject* parent)
    :NDNodeBase(parent)
{
    initAttribute();
}

LightCasterNode::~LightCasterNode()
{

}

void LightCasterNode::initAttribute(void)
{
    // 平行光的材质信息
    initDirectionLight();

    // 点光源的材质信息
    initPointLight();

    // 聚光灯的材质信息
    initSpotLight();

    // 手电筒的材质信息
    initFlashLight();
}

void LightCasterNode::initDirectionLight(void)
{
    NDAttributeGroup* pLightMaterialGroup = new NDAttributeGroup;
    pLightMaterialGroup->setName("DirectionLightMaterial");
    pLightMaterialGroup->setDisplayName(tr("Direction Light Material"));
    this->addAttributeGroup(pLightMaterialGroup);

    m_pLightPostionAttr = new NDVec3Attribute;
    m_pLightPostionAttr->setName("LightDirection");
    m_pLightPostionAttr->setDisplayName(tr("Light Direction: "));
    m_pLightPostionAttr->setRangeValue(-1000.0f, 1000.0f);
    pLightMaterialGroup->addAttribute(m_pLightPostionAttr);
    QObject::connect(m_pLightPostionAttr, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);

    m_pLightAmbientColorAttr = new NDColorAttribute;
    m_pLightAmbientColorAttr->setName("AmbientColor");
    m_pLightAmbientColorAttr->setDisplayName(tr("Ambient Color: "));
    m_pLightAmbientColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pLightAmbientColorAttr);
    QObject::connect(m_pLightAmbientColorAttr, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);

    m_pLightDiffuseColorAttr = new NDColorAttribute;
    m_pLightDiffuseColorAttr->setName("DiffuseColor");
    m_pLightDiffuseColorAttr->setDisplayName(tr("Diffuse Color: "));
    m_pLightDiffuseColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pLightDiffuseColorAttr);
    QObject::connect(m_pLightDiffuseColorAttr, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);

    m_pLightSpecularColorAttr = new NDColorAttribute;
    m_pLightSpecularColorAttr->setName("SpecularColor");
    m_pLightSpecularColorAttr->setDisplayName(tr("Specular Color: "));
    m_pLightSpecularColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pLightSpecularColorAttr);
    QObject::connect(m_pLightSpecularColorAttr, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);
}

void LightCasterNode::initPointLight(void)
{
    NDAttributeGroup* pLightMaterialGroup = new NDAttributeGroup;
    pLightMaterialGroup->setName("PointLightMaterial");
    pLightMaterialGroup->setDisplayName(tr("Point Light Material"));
    this->addAttributeGroup(pLightMaterialGroup);

    m_pPointLightPostionAttr = new NDVec3Attribute;
    m_pPointLightPostionAttr->setName("LightPostion");
    m_pPointLightPostionAttr->setDisplayName(tr("Light Postion: "));
    m_pPointLightPostionAttr->setRangeValue(-1000.0f, 1000.0f);
    pLightMaterialGroup->addAttribute(m_pPointLightPostionAttr);
    QObject::connect(m_pPointLightPostionAttr, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);

    m_pPointLightAmbientColorAttr = new NDColorAttribute;
    m_pPointLightAmbientColorAttr->setName("PointLightAmbientColor");
    m_pPointLightAmbientColorAttr->setDisplayName(tr("Ambient Color: "));
    m_pPointLightAmbientColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pPointLightAmbientColorAttr);
    QObject::connect(m_pPointLightAmbientColorAttr, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);

    m_pPointLightDiffuseColorAttr = new NDColorAttribute;
    m_pPointLightDiffuseColorAttr->setName("PointLightDiffuseColor");
    m_pPointLightDiffuseColorAttr->setDisplayName(tr("Diffuse Color: "));
    m_pPointLightDiffuseColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pPointLightDiffuseColorAttr);
    QObject::connect(m_pPointLightDiffuseColorAttr, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);

    m_pPointLightSpecularColorAttr = new NDColorAttribute;
    m_pPointLightSpecularColorAttr->setName("PointLightSpecularColor");
    m_pPointLightSpecularColorAttr->setDisplayName(tr("Specular Color: "));
    m_pPointLightSpecularColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pPointLightSpecularColorAttr);
    QObject::connect(m_pPointLightSpecularColorAttr, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);
}

void LightCasterNode::initSpotLight(void)
{
    NDAttributeGroup* pLightMaterialGroup = new NDAttributeGroup;
    pLightMaterialGroup->setName("SpotLightMaterial");
    pLightMaterialGroup->setDisplayName(tr("Spot Light Material"));
    this->addAttributeGroup(pLightMaterialGroup);

    m_pSpotLightPostionAttr = new NDVec3Attribute;
    m_pSpotLightPostionAttr->setName("SpotLightPostion");
    m_pSpotLightPostionAttr->setDisplayName(tr("Light Postion: "));
    m_pSpotLightPostionAttr->setRangeValue(-1000.0f, 1000.0f);
    pLightMaterialGroup->addAttribute(m_pSpotLightPostionAttr);
    QObject::connect(m_pSpotLightPostionAttr, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);

    m_pSpotLightDirectionAttr = new NDVec3Attribute;
    m_pSpotLightDirectionAttr->setName("SpotDirection");
    m_pSpotLightDirectionAttr->setDisplayName(tr("Light Direction: "));
    m_pSpotLightDirectionAttr->setRangeValue(-1000.0f, 1000.0f);
    pLightMaterialGroup->addAttribute(m_pSpotLightDirectionAttr);
    QObject::connect(m_pSpotLightDirectionAttr, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);

    m_pSpotLightInnerAttrValue = new NDRealAttribute;
    m_pSpotLightInnerAttrValue->setName("InnerAngle");
    m_pSpotLightInnerAttrValue->setDisplayName(tr("Inner Angle: "));
    m_pSpotLightInnerAttrValue->setValueRange(1.0f, 180.0f);
    m_pSpotLightInnerAttrValue->setValue(12.5f);
    pLightMaterialGroup->addAttribute(m_pSpotLightInnerAttrValue);
    QObject::connect(m_pSpotLightInnerAttrValue, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);

    m_pSpotLightOuterAttrValue = new NDRealAttribute;
    m_pSpotLightOuterAttrValue->setName("OuterAngle");
    m_pSpotLightOuterAttrValue->setDisplayName(tr("Outer Angle: "));
    m_pSpotLightOuterAttrValue->setValueRange(1.0f, 180.0f);
    m_pSpotLightOuterAttrValue->setValue(17.5f);
    pLightMaterialGroup->addAttribute(m_pSpotLightOuterAttrValue);
    QObject::connect(m_pSpotLightOuterAttrValue, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);

    m_pSpotLightAmbientColorAttr = new NDColorAttribute;
    m_pSpotLightAmbientColorAttr->setName("PointLightAmbientColor");
    m_pSpotLightAmbientColorAttr->setDisplayName(tr("Ambient Color: "));
    m_pSpotLightAmbientColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pSpotLightAmbientColorAttr);
    QObject::connect(m_pSpotLightAmbientColorAttr, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);

    m_pSpotLightDiffuseColorAttr = new NDColorAttribute;
    m_pSpotLightDiffuseColorAttr->setName("PointLightDiffuseColor");
    m_pSpotLightDiffuseColorAttr->setDisplayName(tr("Diffuse Color: "));
    m_pSpotLightDiffuseColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pSpotLightDiffuseColorAttr);
    QObject::connect(m_pSpotLightDiffuseColorAttr, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);

    m_pSpotLightSpecularColorAttr = new NDColorAttribute;
    m_pSpotLightSpecularColorAttr->setName("PointLightSpecularColor");
    m_pSpotLightSpecularColorAttr->setDisplayName(tr("Specular Color: "));
    m_pSpotLightSpecularColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pSpotLightSpecularColorAttr);
    QObject::connect(m_pSpotLightSpecularColorAttr, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);
}

void LightCasterNode::initFlashLight(void)
{
    NDAttributeGroup* pLightMaterialGroup = new NDAttributeGroup;
    pLightMaterialGroup->setName("FlashLightMaterial");
    pLightMaterialGroup->setDisplayName(tr("Flash Light Material"));
    this->addAttributeGroup(pLightMaterialGroup);

    m_pFlashLightPostionAttr = new NDVec3Attribute;
    m_pFlashLightPostionAttr->setName("FlashLightPostion");
    m_pFlashLightPostionAttr->setDisplayName(tr("Light Postion: "));
    m_pFlashLightPostionAttr->setRangeValue(-1000.0f, 1000.0f);
    m_pFlashLightPostionAttr->setEnable(false);
    pLightMaterialGroup->addAttribute(m_pFlashLightPostionAttr);\

    m_pFlashLightDirectionAttr = new NDVec3Attribute;
    m_pFlashLightDirectionAttr->setName("FlashDirection");
    m_pFlashLightDirectionAttr->setDisplayName(tr("Light Direction: "));
    m_pFlashLightDirectionAttr->setRangeValue(-1000.0f, 1000.0f);
    m_pFlashLightDirectionAttr->setEnable(false);
    pLightMaterialGroup->addAttribute(m_pFlashLightDirectionAttr);

    m_pFlashLightInnerAttrValue = new NDRealAttribute;
    m_pFlashLightInnerAttrValue->setName("FlashInnerAngle");
    m_pFlashLightInnerAttrValue->setDisplayName(tr("Inner Angle: "));
    m_pFlashLightInnerAttrValue->setValueRange(1.0f, 180.0f);
    m_pFlashLightInnerAttrValue->setValue(12.5f);
    pLightMaterialGroup->addAttribute(m_pFlashLightInnerAttrValue);
    QObject::connect(m_pFlashLightInnerAttrValue, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);

    m_pFlashLightOuterAttrValue = new NDRealAttribute;
    m_pFlashLightOuterAttrValue->setName("FlashOuterAngle");
    m_pFlashLightOuterAttrValue->setDisplayName(tr("Outer Angle: "));
    m_pFlashLightOuterAttrValue->setValueRange(1.0f, 180.0f);
    m_pFlashLightOuterAttrValue->setValue(17.5f);
    pLightMaterialGroup->addAttribute(m_pFlashLightOuterAttrValue);
    QObject::connect(m_pFlashLightOuterAttrValue, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);

    m_pFlashLightAmbientColorAttr = new NDColorAttribute;
    m_pFlashLightAmbientColorAttr->setName("FlashLightAmbientColor");
    m_pFlashLightAmbientColorAttr->setDisplayName(tr("Ambient Color: "));
    m_pFlashLightAmbientColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pFlashLightAmbientColorAttr);
    QObject::connect(m_pFlashLightAmbientColorAttr, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);

    m_pFlashLightDiffuseColorAttr = new NDColorAttribute;
    m_pFlashLightDiffuseColorAttr->setName("FlashLightDiffuseColor");
    m_pFlashLightDiffuseColorAttr->setDisplayName(tr("Diffuse Color: "));
    m_pFlashLightDiffuseColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pFlashLightDiffuseColorAttr);
    QObject::connect(m_pFlashLightDiffuseColorAttr, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);

    m_pFlashLightSpecularColorAttr = new NDColorAttribute;
    m_pFlashLightSpecularColorAttr->setName("FlashLightSpecularColor");
    m_pFlashLightSpecularColorAttr->setDisplayName(tr("Specular Color: "));
    m_pFlashLightSpecularColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pFlashLightSpecularColorAttr);
    QObject::connect(m_pFlashLightSpecularColorAttr, &NDRealAttribute::valueChanged, this, &LightCasterNode::attributeValueChanged);
}

void LightCasterNode::setLightInfo(const DirectionLightRenderWidget::LightInfo& lightInfo)
{
    m_pLightPostionAttr->setValue(lightInfo.lightdirection);

    QColor ambientColor = g_utilTool->vec32Color(lightInfo.ambientColor);
    m_pLightAmbientColorAttr->setValue(ambientColor);

    QColor diffuseColor = g_utilTool->vec32Color(lightInfo.diffuesColor);
    m_pLightDiffuseColorAttr->setValue(diffuseColor);

    QColor specularColor = g_utilTool->vec32Color(lightInfo.specularColor);
    m_pLightSpecularColorAttr->setValue(specularColor);
}

DirectionLightRenderWidget::LightInfo LightCasterNode::getLightInfo(void)
{
    DirectionLightRenderWidget::LightInfo lightInfo;

    QColor ambientColor = m_pLightAmbientColorAttr->getValue().value<QColor>();
    lightInfo.ambientColor = g_utilTool->color2Vec3(ambientColor);

    QColor diffuseColor = m_pLightDiffuseColorAttr->getValue().value<QColor>();
    lightInfo.diffuesColor = g_utilTool->color2Vec3(diffuseColor);

    QColor specularColor = m_pLightSpecularColorAttr->getValue().value<QColor>();
    lightInfo.specularColor = g_utilTool->color2Vec3(specularColor);

    lightInfo.lightdirection = m_pLightPostionAttr->getValue().value<QVector3D>();

    return lightInfo;
}

PointLightRenderWidget::LightInfo LightCasterNode::getPointLightInfo(void)
{
    PointLightRenderWidget::LightInfo lightInfo;

    QColor ambientColor = m_pPointLightAmbientColorAttr->getValue().value<QColor>();
    lightInfo.ambientColor = g_utilTool->color2Vec3(ambientColor);

    QColor diffuseColor = m_pPointLightDiffuseColorAttr->getValue().value<QColor>();
    lightInfo.diffuesColor = g_utilTool->color2Vec3(diffuseColor);

    QColor specularColor = m_pPointLightSpecularColorAttr->getValue().value<QColor>();
    lightInfo.specularColor = g_utilTool->color2Vec3(specularColor);

    lightInfo.lightPostion = m_pPointLightPostionAttr->getValue().value<QVector3D>();

    return lightInfo;
}

void LightCasterNode::setPointLightInfo(const PointLightRenderWidget::LightInfo& lightInfo)
{
    m_pPointLightPostionAttr->setValue(lightInfo.lightPostion);

    QColor ambientColor = g_utilTool->vec32Color(lightInfo.ambientColor);
    m_pPointLightAmbientColorAttr->setValue(ambientColor);

    QColor diffuseColor = g_utilTool->vec32Color(lightInfo.diffuesColor);
    m_pPointLightDiffuseColorAttr->setValue(diffuseColor);

    QColor specularColor = g_utilTool->vec32Color(lightInfo.specularColor);
    m_pPointLightSpecularColorAttr->setValue(specularColor);
}

SpotLightRenderWidget::LightInfo LightCasterNode::getSpotLightInfo(void)
{
    SpotLightRenderWidget::LightInfo lightInfo;

    QColor ambientColor = m_pSpotLightAmbientColorAttr->getValue().value<QColor>();
    lightInfo.ambientColor = g_utilTool->color2Vec3(ambientColor);

    QColor diffuseColor = m_pSpotLightDiffuseColorAttr->getValue().value<QColor>();
    lightInfo.diffuesColor = g_utilTool->color2Vec3(diffuseColor);

    QColor specularColor = m_pSpotLightSpecularColorAttr->getValue().value<QColor>();
    lightInfo.specularColor = g_utilTool->color2Vec3(specularColor);

    lightInfo.lightPostion = m_pSpotLightPostionAttr->getValue().value<QVector3D>();
    lightInfo.direction =  m_pSpotLightDirectionAttr->getValue().value<QVector3D>();
    lightInfo.cutout = m_pSpotLightInnerAttrValue->getValue().toFloat();
    lightInfo.outerCutoff = m_pSpotLightOuterAttrValue->getValue().toFloat();

    return lightInfo;
}

void LightCasterNode::setSpotLightInfo(const SpotLightRenderWidget::LightInfo& lightInfo)
{
    m_pSpotLightPostionAttr->setValue(lightInfo.lightPostion);
    m_pSpotLightDirectionAttr->setValue(lightInfo.direction);
    m_pSpotLightInnerAttrValue->setValue(lightInfo.cutout);
    m_pSpotLightOuterAttrValue->setValue(lightInfo.outerCutoff);

    QColor ambientColor = g_utilTool->vec32Color(lightInfo.ambientColor);
    m_pSpotLightAmbientColorAttr->setValue(ambientColor);

    QColor diffuseColor = g_utilTool->vec32Color(lightInfo.diffuesColor);
    m_pSpotLightDiffuseColorAttr->setValue(diffuseColor);

    QColor specularColor = g_utilTool->vec32Color(lightInfo.specularColor);
    m_pSpotLightSpecularColorAttr->setValue(specularColor);
}

FlashLightRenderWidget::LightInfo LightCasterNode::getFlashLightInfo(void)
{
    FlashLightRenderWidget::LightInfo lightInfo;

    QColor ambientColor = m_pFlashLightAmbientColorAttr->getValue().value<QColor>();
    lightInfo.ambientColor = g_utilTool->color2Vec3(ambientColor);

    QColor diffuseColor = m_pFlashLightDiffuseColorAttr->getValue().value<QColor>();
    lightInfo.diffuesColor = g_utilTool->color2Vec3(diffuseColor);

    QColor specularColor = m_pFlashLightSpecularColorAttr->getValue().value<QColor>();
    lightInfo.specularColor = g_utilTool->color2Vec3(specularColor);

    lightInfo.lightPostion = m_pFlashLightPostionAttr->getValue().value<QVector3D>();
    lightInfo.direction =  m_pFlashLightDirectionAttr->getValue().value<QVector3D>();
    lightInfo.cutout = m_pFlashLightInnerAttrValue  ->getValue().toFloat();
    lightInfo.outerCutoff = m_pFlashLightOuterAttrValue->getValue().toFloat();

    return lightInfo;
}

void LightCasterNode::setFlashLightInfo(const FlashLightRenderWidget::LightInfo& lightInfo)
{
    m_pFlashLightPostionAttr->setValue(lightInfo.lightPostion);
    m_pFlashLightDirectionAttr->setValue(lightInfo.direction);
    m_pFlashLightInnerAttrValue->setValue(lightInfo.cutout);
    m_pFlashLightOuterAttrValue->setValue(lightInfo.outerCutoff);

    QColor ambientColor = g_utilTool->vec32Color(lightInfo.ambientColor);
    m_pFlashLightAmbientColorAttr->setValue(ambientColor);

    QColor diffuseColor = g_utilTool->vec32Color(lightInfo.diffuesColor);
    m_pFlashLightDiffuseColorAttr->setValue(diffuseColor);

    QColor specularColor = g_utilTool->vec32Color(lightInfo.specularColor);
    m_pFlashLightSpecularColorAttr->setValue(specularColor);
}
