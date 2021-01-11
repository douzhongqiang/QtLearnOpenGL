#include "CubeWidget.h"
#include "CubeRenderWidget.h"
#include "CustomCombineControl/UICustomDoubleControl.h"
#include "CustomCombineControl/UICustomVec3Control.h"
#include "CustomCombineControl/UICustomIntControl.h"
#include <QVBoxLayout>
#include <QPushButton>

CubeWidget::CubeWidget(QWidget* parent)
    :QWidget(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(2);
    pLayout->setSpacing(2);

    m_pRenderWidget = new CubeRenderWidget;
    m_pRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pLayout->addWidget(m_pRenderWidget);

    // 添加混合控件
    m_pMinNumberControl = new UICustomDoubleControl;
    m_pMinNumberControl->setColumnWidth(0, 230);
    m_pMinNumberControl->setRangeValue(0, 1);
    m_pMinNumberControl->setTagText(tr("Mix Number: "));
    m_pMinNumberControl->setCurrentValue(m_pRenderWidget->getMixNumber());
    QObject::connect(m_pMinNumberControl, &UICustomDoubleControl::valueChanged, this, &CubeWidget::onValueChanged);
    pLayout->addWidget(m_pMinNumberControl);

    // 添加旋转轴控件
    m_pVecControl = new UICustomVec3Control;
    m_pVecControl->setColumnWidth(0, 230);
    m_pVecControl->setTagText(tr("Rotation Direction: "));
    m_pVecControl->setValue(m_pRenderWidget->getRotationDirection());
    QObject::connect(m_pVecControl, &UICustomVec3Control::valueChanged, this, &CubeWidget::onVecValueChanged);
    pLayout->addWidget(m_pVecControl);

    // 添加速度控件
    m_pSpeedControl = new UICustomIntControl;
    m_pSpeedControl->setColumnWidth(0, 230);
    m_pSpeedControl->setTagText(tr("Speed: "));
    m_pSpeedControl->setRangeValue(0, 20);
    m_pSpeedControl->setCurrentValue(m_pRenderWidget->getSpeed());
    QObject::connect(m_pSpeedControl, &UICustomIntControl::valueChanged, this, &CubeWidget::onSpeedChanged);
    pLayout->addWidget(m_pSpeedControl);
}

CubeWidget::~CubeWidget()
{

}

void CubeWidget::onValueChanged(qreal value, bool cmd)
{
    m_pRenderWidget->setMixNumber(value);
}

void CubeWidget::onVecValueChanged(const QVector3D& vec)
{
    m_pRenderWidget->setRotationDirection(vec);
}

void CubeWidget::onSpeedChanged(int speed)
{
    m_pRenderWidget->setSpeed(speed);
}
