#include "CubeWidget.h"
#include "CubeRenderWidget.h"
#include "CustomCombineControl/UICustomDoubleControl.h"
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

    m_pMinNumberControl = new UICustomDoubleControl;
    m_pMinNumberControl->setRangeValue(0, 1);
    m_pMinNumberControl->setTagText(tr("Mix Number: "));
    m_pMinNumberControl->setCurrentValue(m_pRenderWidget->getMixNumber());
    QObject::connect(m_pMinNumberControl, &UICustomDoubleControl::valueChanged, this, &CubeWidget::onValueChanged);
    pLayout->addWidget(m_pMinNumberControl);
}

CubeWidget::~CubeWidget()
{

}

void CubeWidget::onValueChanged(qreal value, bool cmd)
{
    m_pRenderWidget->setMixNumber(value);
}
