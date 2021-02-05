#include "UIAttrComboBoxControl.h"

UIAttrComboxControl::UIAttrComboxControl(NDAttributeBase* attribute, QWidget* parent)
    :UICustomComboBoxControl(parent)
{
    setAttribute(attribute);

    if (m_attribute == nullptr)
        return;
}

UIAttrComboxControl::~UIAttrComboxControl()
{

}

// 设置属性
void UIAttrComboxControl::setAttribute(NDAttributeBase* attribute)
{
    if (attribute == nullptr || attribute->Type() != NDAttributeBase::t_string2)
        return;

    m_attribute = qobject_cast<NDString2Attribute*>(attribute);
    if (m_attribute == nullptr)
        return;

    // 设置值
    const QList<QPair<QString, QVariant>>& strList = m_attribute->getStringList();
    for (auto iter = strList.begin(); iter != strList.end(); ++iter)
    {
        this->addItem(iter->first, iter->second);
    }
    this->setTagText(m_attribute->getDisplayName());
    this->setCurrentIndex(m_attribute->getCurrentSelectedIndex());

    // 连接信号和槽
    QObject::connect(this, &UIAttrComboxControl::currentIndexChanged, this, &UIAttrComboxControl::onControlCurrentIndexChanged);
    QObject::connect(m_attribute, &NDString2Attribute::currentSelectedIndexChanged, this, &UIAttrComboxControl::onAttributeCurrentIndexChanged);
}

void UIAttrComboxControl::onControlCurrentIndexChanged(int index)
{
    if (m_attribute == nullptr)
        return;

    m_attribute->setCurrentSelectedIndex(index);
}

void UIAttrComboxControl::onAttributeCurrentIndexChanged(int index)
{
    this->blockSignals(true);
    this->setCurrentIndex(index);
    this->blockSignals(false);
}
