#include "NDVec3Attribute.h"

NDVec3Attribute::NDVec3Attribute(NDNodeBase* parentNode)
    :NDAttributeBase(parentNode)
{

}

NDVec3Attribute::~NDVec3Attribute()
{

}

void NDVec3Attribute::setValue(const QVector3D& value, bool cmd)
{
    QVariant variant;
    variant.setValue<QVector3D>(value);

    NDAttributeBase::setValue(variant, cmd);
}

QVector3D NDVec3Attribute::getVecValue(void)
{
    return m_value.value<QVector3D>();
}

void NDVec3Attribute::setRangeValue(float min, float max)
{
    m_minValue = min;
    m_maxValue = max;
}

void NDVec3Attribute::getRangeValue(float& min, float& max)
{
    min = m_minValue;
    max = m_maxValue;
}

float NDVec3Attribute::getMinValue(void)
{
    return m_minValue;
}

float NDVec3Attribute::getMaxValue(void)
{
    return m_maxValue;
}
