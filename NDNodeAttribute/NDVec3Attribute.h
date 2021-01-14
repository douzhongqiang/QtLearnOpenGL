#ifndef MDVEC3ATTRIBUTE_H
#define MDVEC3ATTRIBUTE_H

#include "NDAttributeBase.h"
#include "ndnodeattribute_global.h"
#include <QVector3D>

class NDNODEATTRIBUTESHARED_EXPORT NDVec3Attribute : public NDAttributeBase
{
    Q_OBJECT

public:
    NDVec3Attribute(NDNodeBase* parentNode = nullptr);
    ~NDVec3Attribute();

    AttributeType Type(void) const override{
        return t_vec3;
    }

    void setValue(const QVector3D& value, bool cmd = false);
    QVector3D getVecValue(void);

    void setRangeValue(float min, float max);
    void getRangeValue(float& min, float& max);

    float getMinValue(void);
    float getMaxValue(void);

private:
    float m_minValue = 0.0f;
    float m_maxValue = 1.0f;
};

#endif
