#include "IBOAndPolygonMode.h"
#include <QVBoxLayout>
#include "IBOAndPolygonModeWidget.h"
#include <QPushButton>

IBOAndPolygonMode::IBOAndPolygonMode(QWidget* parent)
    :QWidget(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(2);
    pLayout->setSpacing(2);

    m_pRenderWidget = new IBOAndPolygonModeWidget;
    m_pRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pLayout->addWidget(m_pRenderWidget);

    QPushButton* pButton = new QPushButton(tr("OK"));
    QObject::connect(pButton, &QPushButton::clicked, this, &IBOAndPolygonMode::onClickedButton);
    pButton->setMinimumHeight(40);
    pLayout->addWidget(pButton);
}

IBOAndPolygonMode::~IBOAndPolygonMode()
{

}

void IBOAndPolygonMode::onClickedButton(void)
{
    bool isFilled = m_pRenderWidget->isFill();
    m_pRenderWidget->setFillStatus(!isFilled);
}
