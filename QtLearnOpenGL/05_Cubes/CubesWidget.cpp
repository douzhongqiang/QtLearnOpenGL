#include "CubesWidget.h"
#include "CubesRenderWidget.h"
#include "CustomCombineControl/UICustomDoubleControl.h"
#include "CustomCombineControl/UICustomVec3Control.h"
#include "CustomCombineControl/UICustomIntControl.h"
#include <QVBoxLayout>
#include <QPushButton>

CubesWidget::CubesWidget(QWidget* parent)
    :QWidget(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(2);
    pLayout->setSpacing(2);

    m_pRenderWidget = new CubesRenderWidget;
    m_pRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pLayout->addWidget(m_pRenderWidget);
}

CubesWidget::~CubesWidget()
{

}

