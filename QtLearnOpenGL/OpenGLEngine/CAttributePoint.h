#ifndef CATTRIBUTEPOINT_H
#define CATTRIBUTEPOINT_H

#include <QVector>
class  CAttributePoint
{
public:
    float pos[3];               // 点
    float textureCoord[2];      // 纹理坐标
    float normal[3];            // 法线
};

typedef QVector<CAttributePoint> CAttributePointArray;

#endif
