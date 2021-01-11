#include "UICustomVec3Widget.h"
#include "UICustomDoubleSpinBox.h"
#include <QHBoxLayout>

UICustomVec3Widget::UICustomVec3Widget(QWidget* parent)
    :QWidget(parent)
{
    m_pMainLayout = new QHBoxLayout(this);
    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setMargin(0);

    // 初始化控件
    m_pDoubleSpinBoxX = new UICustomDoubleSpinBox;
    m_pDoubleSpinBoxX->setRange(-1.0, 1.0);
    m_pDoubleSpinBoxX->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_pDoubleSpinBoxY = new UICustomDoubleSpinBox;
    m_pDoubleSpinBoxY->setRange(-1.0, 1.0);
    m_pDoubleSpinBoxY->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_pDoubleSpinBoxZ = new UICustomDoubleSpinBox;
    m_pDoubleSpinBoxZ->setRange(-1.0, 1.0);
    m_pDoubleSpinBoxZ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_pMainLayout->addWidget(m_pDoubleSpinBoxX);
    m_pMainLayout->addWidget(m_pDoubleSpinBoxY);
    m_pMainLayout->addWidget(m_pDoubleSpinBoxZ);

    updateDisplay();
    QObject::connect(m_pDoubleSpinBoxX, SIGNAL(valueChanged(double)), \
                     this, SLOT(onDoubleSpinBoxValueChanged(double)));
    QObject::connect(m_pDoubleSpinBoxY, SIGNAL(valueChanged(double)), \
                     this, SLOT(onDoubleSpinBoxValueChanged(double)));
    QObject::connect(m_pDoubleSpinBoxZ, SIGNAL(valueChanged(double)), \
                     this, SLOT(onDoubleSpinBoxValueChanged(double)));
}

UICustomVec3Widget::~UICustomVec3Widget()
{

}

// 设置间隔
void UICustomVec3Widget::setSpinBoxSpacing(int spacing)
{
    m_pMainLayout->setSpacing(spacing);
}

// 设置Margin
void UICustomVec3Widget::setSpinBoxMargin(const QMargins& margins)
{
    m_pMainLayout->setContentsMargins(margins);
}

void UICustomVec3Widget::setRange(float minValue, float maxValue)
{
    m_pDoubleSpinBoxX->setRange(minValue, maxValue);
    m_pDoubleSpinBoxY->setRange(minValue, maxValue);
    m_pDoubleSpinBoxZ->setRange(minValue, maxValue);
}

// 设置和获取数值
void UICustomVec3Widget::setValue(const QVector3D& vec)
{
    if (m_value != vec)
    {
        m_value = vec;
        updateDisplay();

        emit valueChanged(m_value);
    }
}

QVector3D UICustomVec3Widget::getValue(void)
{
    return m_value;
}

void UICustomVec3Widget::updateDisplay(void)
{
    m_pDoubleSpinBoxX->blockSignals(true);
    m_pDoubleSpinBoxY->blockSignals(true);
    m_pDoubleSpinBoxZ->blockSignals(true);

    m_pDoubleSpinBoxX->setValue(m_value.x());
    m_pDoubleSpinBoxY->setValue(m_value.y());
    m_pDoubleSpinBoxZ->setValue(m_value.z());

    m_pDoubleSpinBoxX->blockSignals(false);
    m_pDoubleSpinBoxY->blockSignals(false);
    m_pDoubleSpinBoxZ->blockSignals(false);
}

void UICustomVec3Widget::updateValue(void)
{
    float xValue = m_pDoubleSpinBoxX->value();
    float yValue = m_pDoubleSpinBoxY->value();
    float zValue = m_pDoubleSpinBoxZ->value();

    m_value.setX(xValue);
    m_value.setY(yValue);
    m_value.setZ(zValue);
}

void UICustomVec3Widget::onDoubleSpinBoxValueChanged(double value)
{
    updateValue();
    emit valueChanged(m_value);
}
