#include "DrawInstanceWidget.h"
#include "DrawInstanceRenderWidget.h"
#include "NDAttributeBase.h"
#include "UINodeAttrControl.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QSplitter>
#include <QFileDialog>

DrawInstanceWidget::DrawInstanceWidget(QWidget* parent)
    :QWidget(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(2);
    pLayout->setSpacing(2);

    m_pRenderWidget = new DrawInstaceRenderWidget;
    pLayout->addWidget(m_pRenderWidget);
}

DrawInstanceWidget::~DrawInstanceWidget()
{

}

