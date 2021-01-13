#ifndef UTILS_H
#define UTILS_H

#include "utils_global.h"
#include <QtCore/QtCore>
#include <QVector3D>
#include <QColor>
#define g_utilTool Utils::getInstance()
#define FLOAT_INTER_VALUE 0.0000001
#define PI 3.14159265358979

class UTILSSHARED_EXPORT Utils
{
public:
    static Utils* getInstance(void);

    // 获取两个点的距离
    qreal getDistance(const QPointF& p1, const QPointF& p2);
    // 比较两个浮点数是否相等
    bool isEqual(qreal number1, qreal number2);

    // 颜色转换
    QVector3D color2Vec3(const QColor& color);
    QColor vec32Color(const QVector3D& vec3);

private:
    Utils();
    virtual ~Utils();
};

#endif // UTILS_H
