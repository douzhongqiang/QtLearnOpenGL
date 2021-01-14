#include "UIAttrVec3Control.h"

UIAttrVec3Control::UIAttrVec3Control(NDAttributeBase* attribute, QWidget* parent)
    :UICustomVec3Control(parent)
{
    setAttribute(attribute);
}

UIAttrVec3Control::~UIAttrVec3Control()
{

}

// 设置属性
void UIAttrVec3Control::setAttribute(NDAttributeBase* attribute)
{
    if (attribute == nullptr || attribute->Type() != NDAttributeBase::t_vec3)
        return;

    m_attribute = qobject_cast<NDVec3Attribute*>(attribute);
    if (m_attribute == nullptr)
        return;

    QVector3D vec = m_attribute->getVecValue();
    this->setValue(vec);

    // 设置取值范围
    this->setRange(m_attribute->getMinValue(), m_attribute->getMaxValue());

    this->setTagText(m_attribute->getDisplayName());

    QObject::connect(m_attribute, &NDVec3Attribute::valueChanged, this, &UIAttrVec3Control::onAttributeValueChanged);
    QObject::connect(this, &UIAttrVec3Control::valueChanged, this, &UIAttrVec3Control::onControlValueChanged);
}

void UIAttrVec3Control::onAttributeValueChanged(const QVariant& value)
{
    this->blockSignals(true);
    this->setValue(value.value<QVector3D>());
    this->blockSignals(false);
}

void UIAttrVec3Control::onControlValueChanged(const QVector3D& value)
{
    QObject::disconnect(m_attribute, &NDVec3Attribute::valueChanged, this, &UIAttrVec3Control::onAttributeValueChanged);
    m_attribute->setValue(value);
    QObject::connect(m_attribute, &NDVec3Attribute::valueChanged, this, &UIAttrVec3Control::onAttributeValueChanged);
}
