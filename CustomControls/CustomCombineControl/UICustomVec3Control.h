#ifndef UICUSTOMVEC3CONTROL_H
#define UICUSTOMVEC3CONTROL_H

#include <QWidget>
#include "UICustomCombineControlBase.h"
#include "customcontrols_global.h"
#include "UICustomSingleControl/UICustomVec3Widget.h"

class CUSTOMCONTROLSSHARED_EXPORT UICustomVec3Control : public UICustomCombineControlBase
{
    Q_OBJECT

public:
    UICustomVec3Control(QWidget* parent = nullptr);
    ~UICustomVec3Control();

    // 设置和获取数值
    void setValue(const QVector3D& vec);
    QVector3D getValue(void);

    // 设置取值范围
    void setRange(float minValue, float maxValue);

private:
    UICustomVec3Widget* m_pVec3Widget = nullptr;

signals:
    void valueChanged(const QVector3D& vec);
};

#endif
