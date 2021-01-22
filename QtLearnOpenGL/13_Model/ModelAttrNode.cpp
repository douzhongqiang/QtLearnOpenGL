#include "ModelAttrNode.h"
#include "NDColorAttribute.h"
#include "NDRealAttribute.h"
#include "NDAttributeGroup.h"
#include "NDVec3Attribute.h"
#include "Utils.h"

ModelAttrNode::ModelAttrNode(QObject* parent)
    :NDNodeBase(parent)
{
    initAttribute();
}

ModelAttrNode::~ModelAttrNode()
{

}

void ModelAttrNode::initAttribute(void)
{
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
    QObject::connect(m_pLightPostionAttr, &NDRealAttribute::valueChanged, this, &ModelAttrNode::attributeValueChanged);

    m_pLightAmbientColorAttr = new NDColorAttribute;
    m_pLightAmbientColorAttr->setName("AmbientColor");
    m_pLightAmbientColorAttr->setDisplayName(tr("Ambient Color: "));
    m_pLightAmbientColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pLightAmbientColorAttr);
    QObject::connect(m_pLightAmbientColorAttr, &NDRealAttribute::valueChanged, this, &ModelAttrNode::attributeValueChanged);

    m_pLightDiffuseColorAttr = new NDColorAttribute;
    m_pLightDiffuseColorAttr->setName("DiffuseColor");
    m_pLightDiffuseColorAttr->setDisplayName(tr("Diffuse Color: "));
    m_pLightDiffuseColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pLightDiffuseColorAttr);
    QObject::connect(m_pLightDiffuseColorAttr, &NDRealAttribute::valueChanged, this, &ModelAttrNode::attributeValueChanged);

    m_pLightSpecularColorAttr = new NDColorAttribute;
    m_pLightSpecularColorAttr->setName("SpecularColor");
    m_pLightSpecularColorAttr->setDisplayName(tr("Specular Color: "));
    m_pLightSpecularColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pLightSpecularColorAttr);
    QObject::connect(m_pLightSpecularColorAttr, &NDRealAttribute::valueChanged, this, &ModelAttrNode::attributeValueChanged);

}

void ModelAttrNode::setLightInfo(const ModelRenderWidget::LightInfo& lightInfo)
{
    m_pLightPostionAttr->setValue(lightInfo.lightPos);

    QColor ambientColor = g_utilTool->vec32Color(lightInfo.ambientColor);
    m_pLightAmbientColorAttr->setValue(ambientColor);

    QColor diffuseColor = g_utilTool->vec32Color(lightInfo.diffuesColor);
    m_pLightDiffuseColorAttr->setValue(diffuseColor);

    QColor specularColor = g_utilTool->vec32Color(lightInfo.specularColor);
    m_pLightSpecularColorAttr->setValue(specularColor);
}

ModelRenderWidget::LightInfo ModelAttrNode::getLightInfo(void)
{
    ModelRenderWidget::LightInfo lightInfo;

    QColor ambientColor = m_pLightAmbientColorAttr->getValue().value<QColor>();
    lightInfo.ambientColor = g_utilTool->color2Vec3(ambientColor);

    QColor diffuseColor = m_pLightDiffuseColorAttr->getValue().value<QColor>();
    lightInfo.diffuesColor = g_utilTool->color2Vec3(diffuseColor);

    QColor specularColor = m_pLightSpecularColorAttr->getValue().value<QColor>();
    lightInfo.specularColor = g_utilTool->color2Vec3(specularColor);

    lightInfo.lightPos = m_pLightPostionAttr->getValue().value<QVector3D>();

    return lightInfo;
}
