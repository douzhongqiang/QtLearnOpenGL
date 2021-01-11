#include "UICustomVec3Control.h"

UICustomVec3Control::UICustomVec3Control(QWidget* parent)
    :UICustomCombineControlBase(parent)
{
    m_pVec3Widget = new UICustomVec3Widget;
    m_pMainLayout->addWidget(m_pVec3Widget);
    m_pVec3Widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    this->setWidth(400);
    this->setHeight(30);

    m_pVec3Widget->setSpinBoxSpacing(2);

    QObject::connect(m_pVec3Widget, &UICustomVec3Widget::valueChanged, this, &UICustomVec3Control::valueChanged);
}

UICustomVec3Control::~UICustomVec3Control()
{

}

// 设置和获取数值
void UICustomVec3Control::setValue(const QVector3D& vec)
{
    m_pVec3Widget->setValue(vec);
}

QVector3D UICustomVec3Control::getValue(void)
{
    return m_pVec3Widget->getValue();
}

void UICustomVec3Control::setRange(float minValue, float maxValue)
{
    m_pVec3Widget->setRange(minValue, maxValue);
}
