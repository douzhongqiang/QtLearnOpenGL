#include "FrameBufferAttrNode.h"
#include "NDColorAttribute.h"
#include "NDRealAttribute.h"
#include "NDAttributeGroup.h"
#include "NDVec3Attribute.h"
#include "NDBoolAttribute.h"
#include "NDString2Attribute.h"
#include "Utils.h"

FrameBufferAttrNode::FrameBufferAttrNode(QObject* parent)
    :NDNodeBase(parent)
{
    initAttribute();
}

FrameBufferAttrNode::~FrameBufferAttrNode()
{

}

void FrameBufferAttrNode::initAttribute(void)
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
    QObject::connect(m_pLightPostionAttr, &NDRealAttribute::valueChanged, this, &FrameBufferAttrNode::attributeValueChanged);

    m_pLightAmbientColorAttr = new NDColorAttribute;
    m_pLightAmbientColorAttr->setName("AmbientColor");
    m_pLightAmbientColorAttr->setDisplayName(tr("Ambient Color: "));
    m_pLightAmbientColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pLightAmbientColorAttr);
    QObject::connect(m_pLightAmbientColorAttr, &NDRealAttribute::valueChanged, this, &FrameBufferAttrNode::attributeValueChanged);

    m_pLightDiffuseColorAttr = new NDColorAttribute;
    m_pLightDiffuseColorAttr->setName("DiffuseColor");
    m_pLightDiffuseColorAttr->setDisplayName(tr("Diffuse Color: "));
    m_pLightDiffuseColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pLightDiffuseColorAttr);
    QObject::connect(m_pLightDiffuseColorAttr, &NDRealAttribute::valueChanged, this, &FrameBufferAttrNode::attributeValueChanged);

    m_pLightSpecularColorAttr = new NDColorAttribute;
    m_pLightSpecularColorAttr->setName("SpecularColor");
    m_pLightSpecularColorAttr->setDisplayName(tr("Specular Color: "));
    m_pLightSpecularColorAttr->setValue(QColor(200, 100, 100));
    pLightMaterialGroup->addAttribute(m_pLightSpecularColorAttr);
    QObject::connect(m_pLightSpecularColorAttr, &NDRealAttribute::valueChanged, this, &FrameBufferAttrNode::attributeValueChanged);

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
    QObject::connect(m_pCameraPosAttr, &NDRealAttribute::valueChanged, this, &FrameBufferAttrNode::attributeValueChanged);

    // 相机的方向
    m_pCameraFront = new NDVec3Attribute;
    m_pCameraFront->setName("CameraFront");
    m_pCameraFront->setDisplayName(tr("Camera Front: "));
    m_pCameraFront->setRangeValue(-1000.0f, 1000.0f);
    pCameraGroup->addAttribute(m_pCameraFront);
    QObject::connect(m_pCameraFront, &NDRealAttribute::valueChanged, this, &FrameBufferAttrNode::attributeValueChanged);

    // 添加面剔除属性组
    this->addAttributeGroup(this->getCullFaceAttrGroup());
}

NDAttributeGroup* FrameBufferAttrNode::getCullFaceAttrGroup(void)
{
    // 面剔除
    NDAttributeGroup* pCullFaceAttrGroup = new NDAttributeGroup;
    pCullFaceAttrGroup->setName("CullFace");
    pCullFaceAttrGroup->setDisplayName(tr("Cull Face"));

    // 是否开启面剔除
    m_pCullFaceEnabledAttr = new NDBoolAttribute;
    m_pCullFaceEnabledAttr->setName("CullFaceEnabled");
    m_pCullFaceEnabledAttr->setDisplayName(tr("Cull Face Enable: "));
    m_pCullFaceEnabledAttr->setValue(false);
    pCullFaceAttrGroup->addAttribute(m_pCullFaceEnabledAttr);
    QObject::connect(m_pCullFaceEnabledAttr, &NDBoolAttribute::valueChanged, this, &FrameBufferAttrNode::attributeValueChanged);

    // 面剔除的类型 - 剔除哪个面
    m_pCullFaceTypeAttr = new NDString2Attribute;
    m_pCullFaceTypeAttr->setName("CullFaceType");
    m_pCullFaceTypeAttr->setDisplayName(tr("Cull Face Type: "));
    QList<QPair<QString, QVariant>> cullfaceTypeStringList;
    cullfaceTypeStringList << QPair<QString, QVariant>("back", 0) << QPair<QString, QVariant>("front", 1) << QPair<QString, QVariant>("back&front", 2);
    m_pCullFaceTypeAttr->setStringList(cullfaceTypeStringList);
    m_pCullFaceTypeAttr->setCurrentSelectedIndex(0);
    pCullFaceAttrGroup->addAttribute(m_pCullFaceTypeAttr);
    QObject::connect(m_pCullFaceTypeAttr, &NDString2Attribute::valueChanged, this, &FrameBufferAttrNode::attributeValueChanged);

    // 前面的绘制顺序 - 顺时针/逆时针
    m_pFrontFaceTypeAttr = new NDString2Attribute;
    m_pFrontFaceTypeAttr->setName("FrontFaceType");
    m_pFrontFaceTypeAttr->setDisplayName(tr("Front Face Type: "));
    QList<QPair<QString, QVariant>> frontFaceTypeStringList;
    frontFaceTypeStringList << QPair<QString, QVariant>("CCW", 0) << QPair<QString, QVariant>("CW", 1);
    m_pFrontFaceTypeAttr->setStringList(frontFaceTypeStringList);
    m_pFrontFaceTypeAttr->setCurrentSelectedIndex(0);
    pCullFaceAttrGroup->addAttribute(m_pFrontFaceTypeAttr);
    QObject::connect(m_pFrontFaceTypeAttr, &NDString2Attribute::valueChanged, this, &FrameBufferAttrNode::attributeValueChanged);

    return pCullFaceAttrGroup;
}

void FrameBufferAttrNode::setLightInfo(const FrameBufferRenderWidget::LightInfo& lightInfo)
{
    m_pLightPostionAttr->setValue(lightInfo.lightPos);

    QColor ambientColor = g_utilTool->vec32Color(lightInfo.ambientColor);
    m_pLightAmbientColorAttr->setValue(ambientColor);

    QColor diffuseColor = g_utilTool->vec32Color(lightInfo.diffuesColor);
    m_pLightDiffuseColorAttr->setValue(diffuseColor);

    QColor specularColor = g_utilTool->vec32Color(lightInfo.specularColor);
    m_pLightSpecularColorAttr->setValue(specularColor);
}

FrameBufferRenderWidget::LightInfo FrameBufferAttrNode::getLightInfo(void)
{
    FrameBufferRenderWidget::LightInfo lightInfo;

    QColor ambientColor = m_pLightAmbientColorAttr->getValue().value<QColor>();
    lightInfo.ambientColor = g_utilTool->color2Vec3(ambientColor);

    QColor diffuseColor = m_pLightDiffuseColorAttr->getValue().value<QColor>();
    lightInfo.diffuesColor = g_utilTool->color2Vec3(diffuseColor);

    QColor specularColor = m_pLightSpecularColorAttr->getValue().value<QColor>();
    lightInfo.specularColor = g_utilTool->color2Vec3(specularColor);

    lightInfo.lightPos = m_pLightPostionAttr->getValue().value<QVector3D>();

    return lightInfo;
}

void FrameBufferAttrNode::setCameraPostion(const QVector3D& cameraPos)
{
    m_pCameraPosAttr->setValue(cameraPos);
}

QVector3D FrameBufferAttrNode::getCameraPostion()
{
    return m_pCameraPosAttr->getValue().value<QVector3D>();
}

void FrameBufferAttrNode::setCameraFront(const QVector3D& cameraFront)
{
    m_pCameraFront->setValue(cameraFront);
}

QVector3D FrameBufferAttrNode::getCameraFront(void)
{
    return m_pCameraFront->getValue().value<QVector3D>();
}

// 面剔除相关
void FrameBufferAttrNode::setCullFaceEnabled(bool isEnabled)
{
    m_pCullFaceEnabledAttr->setValue(isEnabled);
}

bool FrameBufferAttrNode::isCullFaceEnabled(void)
{
    return m_pCullFaceEnabledAttr->getValue().toBool();
}

// 设置要剔除的面
void FrameBufferAttrNode::setCullFaceType(FrameBufferRenderWidget::CullFaceType type)
{
    m_pCullFaceTypeAttr->setCurrentSelectedIndex((int)type);
}

FrameBufferRenderWidget::CullFaceType FrameBufferAttrNode::getCullFaceType(void)
{
    return (FrameBufferRenderWidget::CullFaceType)m_pCullFaceTypeAttr->getCurrentSelectedIndex();
}

// 设置正面的顺序
void FrameBufferAttrNode::setFrontOrderType(FrameBufferRenderWidget::FrontFaceOrderType type)
{
    m_pFrontFaceTypeAttr->setCurrentSelectedIndex((int)type);
}

FrameBufferRenderWidget::FrontFaceOrderType FrameBufferAttrNode::getFrontOrderType(void)
{
    return (FrameBufferRenderWidget::FrontFaceOrderType)m_pFrontFaceTypeAttr->getCurrentSelectedIndex();
}
