#include "CameraWidget.h"
#include "CameraRenderWidget.h"
#include <QVBoxLayout>
#include <QPushButton>

CameraWidget::CameraWidget(QWidget* parent)
    :QWidget(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(2);
    pLayout->setSpacing(2);

    m_pRenderWidget = new CameraRenderWidget;
    m_pRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pLayout->addWidget(m_pRenderWidget);
}

CameraWidget::~CameraWidget()
{

}

