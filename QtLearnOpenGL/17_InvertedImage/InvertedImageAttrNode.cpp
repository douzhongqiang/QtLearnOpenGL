#include "InvertedImageAttrNode.h"
#include "NDColorAttribute.h"
#include "NDRealAttribute.h"
#include "NDAttributeGroup.h"
#include "NDVec3Attribute.h"
#include "Utils.h"

InvertedImageAttrNode::InvertedImageAttrNode(QObject* parent)
    :NDNodeBase(parent)
{
    initAttribute();
}

InvertedImageAttrNode::~InvertedImageAttrNode()
{

}

void InvertedImageAttrNode::initAttribute(void)
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
    QObject::connect(m_pLightPostionAttr, &NDRealAttribute::valueChanged, this, &InvertedImageAttrNode::attributeValueChanged);

    m_pLightAmbientColorAttr = new NDColorAttribute;
    m_pLightAmbientColorAttr->setName("AmbientColor");
    m_pLightAmbientColorAttr->setDisplayName(tr("Ambient Color: "));
    m_pLightAmbientColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pLightAmbientColorAttr);
    QObject::connect(m_pLightAmbientColorAttr, &NDRealAttribute::valueChanged, this, &InvertedImageAttrNode::attributeValueChanged);

    m_pLightDiffuseColorAttr = new NDColorAttribute;
    m_pLightDiffuseColorAttr->setName("DiffuseColor");
    m_pLightDiffuseColorAttr->setDisplayName(tr("Diffuse Color: "));
    m_pLightDiffuseColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pLightDiffuseColorAttr);
    QObject::connect(m_pLightDiffuseColorAttr, &NDRealAttribute::valueChanged, this, &InvertedImageAttrNode::attributeValueChanged);

    m_pLightSpecularColorAttr = new NDColorAttribute;
    m_pLightSpecularColorAttr->setName("SpecularColor");
    m_pLightSpecularColorAttr->setDisplayName(tr("Specular Color: "));
    m_pLightSpecularColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pLightSpecularColorAttr);
    QObject::connect(m_pLightSpecularColorAttr, &NDRealAttribute::valueChanged, this, &InvertedImageAttrNode::attributeValueChanged);

    // 添加相机属性
    NDAttributeGroup* pCameraGroup = new NDAttributeGroup;
    pCameraGroup->setName("CameraInfo");
    pCameraGroup->setDisplayName(tr("CameraInfo"));
    this->addAttributeGroup(pCameraGroup);

    // 相机的位置
    m_pCameraPosAttr = new NDVec3Attribute;
    m_pCameraPosAttr->setName("CameraPostion");
    m_pCameraPosAttr->setDisplayName(tr("Camera Postion: "));
    m_pCameraPosAttr->setRangeValue(-1000.0f, 1000.0f);
    pCameraGroup->addAttribute(m_pCameraPosAttr);
    QObject::connect(m_pCameraPosAttr, &NDRealAttribute::valueChanged, this, &InvertedImageAttrNode::attributeValueChanged);

    // 相机的方向
    m_pCameraFront = new NDVec3Attribute;
    m_pCameraFront->setName("CameraFront");
    m_pCameraFront->setDisplayName(tr("Camera Front: "));
    m_pCameraFront->setRangeValue(-1000.0f, 1000.0f);
    pCameraGroup->addAttribute(m_pCameraFront);
    QObject::connect(m_pCameraFront, &NDRealAttribute::valueChanged, this, &InvertedImageAttrNode::attributeValueChanged);
}

void InvertedImageAttrNode::setLightInfo(const InvertedImageRenderWidget::LightInfo& lightInfo)
{
    m_pLightPostionAttr->setValue(lightInfo.lightPos);

    QColor ambientColor = g_utilTool->vec32Color(lightInfo.ambientColor);
    m_pLightAmbientColorAttr->setValue(ambientColor);

    QColor diffuseColor = g_utilTool->vec32Color(lightInfo.diffuesColor);
    m_pLightDiffuseColorAttr->setValue(diffuseColor);

    QColor specularColor = g_utilTool->vec32Color(lightInfo.specularColor);
    m_pLightSpecularColorAttr->setValue(specularColor);
}

InvertedImageRenderWidget::LightInfo InvertedImageAttrNode::getLightInfo(void)
{
    InvertedImageRenderWidget::LightInfo lightInfo;

    QColor ambientColor = m_pLightAmbientColorAttr->getValue().value<QColor>();
    lightInfo.ambientColor = g_utilTool->color2Vec3(ambientColor);

    QColor diffuseColor = m_pLightDiffuseColorAttr->getValue().value<QColor>();
    lightInfo.diffuesColor = g_utilTool->color2Vec3(diffuseColor);

    QColor specularColor = m_pLightSpecularColorAttr->getValue().value<QColor>();
    lightInfo.specularColor = g_utilTool->color2Vec3(specularColor);

    lightInfo.lightPos = m_pLightPostionAttr->getValue().value<QVector3D>();

    return lightInfo;
}

void InvertedImageAttrNode::setCameraPostion(const QVector3D& cameraPos)
{
    m_pCameraPosAttr->setValue(cameraPos);
}

QVector3D InvertedImageAttrNode::getCameraPostion()
{
    return m_pCameraPosAttr->getValue().value<QVector3D>();
}

void InvertedImageAttrNode::setCameraFront(const QVector3D& cameraFront)
{
    m_pCameraFront->setValue(cameraFront);
}

QVector3D InvertedImageAttrNode::getCameraFront(void)
{
    return m_pCameraFront->getValue().value<QVector3D>();
}
