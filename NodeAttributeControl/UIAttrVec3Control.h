#ifndef UIATTRVEC3CONTROL_H
#define UIATTRVEC3CONTROL_H

#include <QWidget>
#include "NDVec3Attribute.h"
#include "CustomCombineControl/UICustomVec3Control.h"
#include <QVector3D>

class UIAttrVec3Control : public UICustomVec3Control
{
    Q_OBJECT

public:
    UIAttrVec3Control(NDAttributeBase* attribute = nullptr, QWidget* parent = nullptr);
    ~UIAttrVec3Control();

    // 设置属性
    void setAttribute(NDAttributeBase* attribute);

private:
    NDVec3Attribute* m_attribute = nullptr;

private slots:
    void onAttributeValueChanged(const QVariant& value);
    void onControlValueChanged(const QVector3D& value);

};

#endif
