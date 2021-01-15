#include "LightMapNode.h"
#include "NDColorAttribute.h"
#include "NDRealAttribute.h"
#include "NDAttributeGroup.h"
#include "NDVec3Attribute.h"
#include "Utils.h"

LightMapNode::LightMapNode(QObject* parent)
    :NDNodeBase(parent)
{
    initAttribute();
}

LightMapNode::~LightMapNode()
{

}

void LightMapNode::initAttribute(void)
{
    // 添加物体的材质
    NDAttributeGroup* pObjectMaterialGroup = new NDAttributeGroup;
    pObjectMaterialGroup->setName("ObjectMaterial");
    pObjectMaterialGroup->setDisplayName(tr("Object Material"));
    this->addAttributeGroup(pObjectMaterialGroup);

    // 物体的环境光
//    m_pAmbientColorAttr = new NDColorAttribute;
//    m_pAmbientColorAttr->setName("AmbientColor");
//    m_pAmbientColorAttr->setDisplayName(tr("Ambient Color: "));
//    m_pAmbientColorAttr->setValue(QColor(200, 100, 100));
//    pObjectMaterialGroup->addAttribute(m_pAmbientColorAttr);
//    QObject::connect(m_pAmbientColorAttr, &NDColorAttribute::valueChanged, this, &LightMapNode::attributeValueChanged);

//    // 物体的漫反射
//    m_pDiffuseColorAttr = new NDColorAttribute;
//    m_pDiffuseColorAttr->setName("DiffuseColor");
//    m_pDiffuseColorAttr->setDisplayName(tr("Diffuse Color: "));
//    m_pDiffuseColorAttr->setValue(QColor(200, 100, 100));
//    pObjectMaterialGroup->addAttribute(m_pDiffuseColorAttr);
//    QObject::connect(m_pDiffuseColorAttr, &NDColorAttribute::valueChanged, this, &LightMapNode::attributeValueChanged);

//    // 物体的镜面反射
//    m_pSpecularColorAttr = new NDColorAttribute;
//    m_pSpecularColorAttr->setName("SpecularColor");
//    m_pSpecularColorAttr->setDisplayName(tr("Specular Color: "));
//    m_pSpecularColorAttr->setValue(QColor(200, 100, 100));
//    pObjectMaterialGroup->addAttribute(m_pSpecularColorAttr);
//    QObject::connect(m_pSpecularColorAttr, &NDColorAttribute::valueChanged, this, &LightMapNode::attributeValueChanged);

    // 物体的镜面反射系数
    m_pShininessAtr = new NDRealAttribute;
    m_pShininessAtr->setName("Shininess");
    m_pShininessAtr->setDisplayName(tr("Shininess: "));
    m_pShininessAtr->setValueRange(1.0f, 256.0f);
    m_pShininessAtr->setValue(32);
    pObjectMaterialGroup->addAttribute(m_pShininessAtr);
    QObject::connect(m_pShininessAtr, &NDRealAttribute::valueChanged, this, &LightMapNode::attributeValueChanged);

    // =====================================================================================
    // 添加光的材质信息
    NDAttributeGroup* pLightMaterialGroup = new NDAttributeGroup;
    pLightMaterialGroup->setName("LightMaterial");
    pLightMaterialGroup->setDisplayName(tr("Light Material"));
    this->addAttributeGroup(pLightMaterialGroup);

    m_pLightPostionAttr = new NDVec3Attribute;
    m_pLightPostionAttr->setName("LightPostion");
    m_pLightPostionAttr->setDisplayName(tr("Light Postion: "));
    m_pLightPostionAttr->setRangeValue(-1000.0f, 1000.0f);
    pLightMaterialGroup->addAttribute(m_pLightPostionAttr);
    QObject::connect(m_pLightPostionAttr, &NDRealAttribute::valueChanged, this, &LightMapNode::attributeValueChanged);

    m_pLightAmbientColorAttr = new NDColorAttribute;
    m_pLightAmbientColorAttr->setName("AmbientColor");
    m_pLightAmbientColorAttr->setDisplayName(tr("Ambient Color: "));
    m_pLightAmbientColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pLightAmbientColorAttr);
    QObject::connect(m_pLightAmbientColorAttr, &NDRealAttribute::valueChanged, this, &LightMapNode::attributeValueChanged);

    m_pLightDiffuseColorAttr = new NDColorAttribute;
    m_pLightDiffuseColorAttr->setName("DiffuseColor");
    m_pLightDiffuseColorAttr->setDisplayName(tr("Diffuse Color: "));
    m_pLightDiffuseColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pLightDiffuseColorAttr);
    QObject::connect(m_pLightDiffuseColorAttr, &NDRealAttribute::valueChanged, this, &LightMapNode::attributeValueChanged);

    m_pLightSpecularColorAttr = new NDColorAttribute;
    m_pLightSpecularColorAttr->setName("SpecularColor");
    m_pLightSpecularColorAttr->setDisplayName(tr("Specular Color: "));
    m_pLightSpecularColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pLightSpecularColorAttr);
    QObject::connect(m_pLightSpecularColorAttr, &NDRealAttribute::valueChanged, this, &LightMapNode::attributeValueChanged);

}

void LightMapNode::setObjectMaterial(const LightMapRenderWidget::ObjectMaterial& objectMaterial)
{
    // ambient
//    QColor ambientColor = g_utilTool->vec32Color(objectMaterial.ambientColor);
//    m_pAmbientColorAttr->setValue(ambientColor);

//    QColor diffuseColor = g_utilTool->vec32Color(objectMaterial.diffuesColor);
//    m_pDiffuseColorAttr->setValue(diffuseColor);

//    QColor specularColor = g_utilTool->vec32Color(objectMaterial.specularColor);
//    m_pSpecularColorAttr->setValue(specularColor);

    float shininessValue = objectMaterial.shininess;
    m_pShininessAtr->setValue(shininessValue);
}

void LightMapNode::setLightInfo(const LightMapRenderWidget::LightInfo& lightInfo)
{
    m_pLightPostionAttr->setValue(lightInfo.lightPos);

    QColor ambientColor = g_utilTool->vec32Color(lightInfo.ambientColor);
    m_pLightAmbientColorAttr->setValue(ambientColor);

    QColor diffuseColor = g_utilTool->vec32Color(lightInfo.diffuesColor);
    m_pLightDiffuseColorAttr->setValue(diffuseColor);

    QColor specularColor = g_utilTool->vec32Color(lightInfo.specularColor);
    m_pLightSpecularColorAttr->setValue(specularColor);
}

LightMapRenderWidget::ObjectMaterial LightMapNode::getObjectMaterial(void)
{
    LightMapRenderWidget::ObjectMaterial objectMaterial;

//    QColor ambientColor = m_pAmbientColorAttr->getValue().value<QColor>();
//    objectMaterial.ambientColor = g_utilTool->color2Vec3(ambientColor);

//    QColor diffuseColor = m_pDiffuseColorAttr->getValue().value<QColor>();
//    objectMaterial.diffuesColor = g_utilTool->color2Vec3(diffuseColor);

//    QColor specularColor = m_pSpecularColorAttr->getValue().value<QColor>();
//    objectMaterial.specularColor = g_utilTool->color2Vec3(specularColor);

    objectMaterial.shininess = m_pShininessAtr->getValue().toFloat();

    return objectMaterial;
}

LightMapRenderWidget::LightInfo LightMapNode::getLightInfo(void)
{
    LightMapRenderWidget::LightInfo lightInfo;

    QColor ambientColor = m_pLightAmbientColorAttr->getValue().value<QColor>();
    lightInfo.ambientColor = g_utilTool->color2Vec3(ambientColor);

    QColor diffuseColor = m_pLightDiffuseColorAttr->getValue().value<QColor>();
    lightInfo.diffuesColor = g_utilTool->color2Vec3(diffuseColor);

    QColor specularColor = m_pLightSpecularColorAttr->getValue().value<QColor>();
    lightInfo.specularColor = g_utilTool->color2Vec3(specularColor);

    lightInfo.lightPos = m_pLightPostionAttr->getValue().value<QVector3D>();

    return lightInfo;
}
