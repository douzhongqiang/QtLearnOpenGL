#ifndef UIATTRCOMBOBOXCONTROL_H
#define UIATTRCOMBOBOXCONTROL_H

#include "CustomCombineControl/UICustomComboBoxControl.h"
#include "NDString2Attribute.h"
#include <QString>

class UIAttrComboxControl : public UICustomComboBoxControl
{
    Q_OBJECT

public:
    UIAttrComboxControl(NDAttributeBase* attribute = nullptr, QWidget* parent = nullptr);
    ~UIAttrComboxControl();

    // 设置属性
    void setAttribute(NDAttributeBase* attribute);

private:
    NDString2Attribute* m_attribute = nullptr;

private slots:
    void onControlCurrentIndexChanged(int index);
    void onAttributeCurrentIndexChanged(int index);
};

#endif
