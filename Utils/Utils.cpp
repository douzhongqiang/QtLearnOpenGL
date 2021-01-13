#include "Utils.h"

Utils::Utils()
{

}

Utils::~Utils()
{

}

Utils* Utils::getInstance(void)
{
    static Utils instance;
    return &instance;
}

// 获取两个点的距离
qreal Utils::getDistance(const QPointF& p1, const QPointF& p2)
{
    qreal distance = sqrt((p1.x() - p2.x()) * (p1.x() - p2.x()) + \
                          (p1.y() - p2.y()) * (p1.y() - p2.y()));

    return distance;
}

// 比较两个浮点数是否相等
bool Utils::isEqual(qreal number1, qreal number2)
{
    if (abs(number1 - number2) <= FLOAT_INTER_VALUE)
        return true;

    return false;
}

// 颜色转换
QVector3D Utils::color2Vec3(const QColor& color)
{
    QVector3D vec3;

    vec3.setX(color.redF());
    vec3.setY(color.greenF());
    vec3.setZ(color.blueF());

    return vec3;
}

QColor Utils::vec32Color(const QVector3D& vec3)
{
    QColor color;

    color.setRedF(vec3.x());
    color.setGreenF(vec3.y());
    color.setBlueF(vec3.z());

    return color;
}
