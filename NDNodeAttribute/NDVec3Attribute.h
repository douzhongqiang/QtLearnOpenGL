#ifndef MDVEC3ATTRIBUTE_H
#define MDVEC3ATTRIBUTE_H

#include "NDAttributeBase.h"
#include "ndnodeattribute_global.h"

class NDNODEATTRIBUTESHARED_EXPORT NDVec3Attribute : public NDAttributeBase
{
    Q_OBJECT

public:
    NDVec3Attribute(NDNodeBase* parentNode = nullptr);
    ~NDVec3Attribute();

    AttributeType Type(void) const override{
        return t_vec3;
    }
};

#endif
