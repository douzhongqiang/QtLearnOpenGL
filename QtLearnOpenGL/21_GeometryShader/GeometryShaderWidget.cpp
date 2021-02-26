#include "GeometryShaderWidget.h"
#include "GeometryShaderRenderWidget.h"
#include "GeometryShaderRenderWidget2.h"
#include "GeometryShaderRenderWidget3.h"
#include "GeometryShaderRenderWidget4.h"
#include "NDAttributeBase.h"
#include "UINodeAttrControl.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QSplitter>
#include <QFileDialog>

GeometryShaderWidget::GeometryShaderWidget(QWidget* parent)
    :QWidget(parent)
{
    QGridLayout* pLayout = new QGridLayout(this);
    pLayout->setMargin(2);
    pLayout->setSpacing(2);

    m_pRenderWidget = new GeometryShaderRenderWidget;
    pLayout->addWidget(m_pRenderWidget, 0, 0, 1, 1);

    m_pRenderWidget2 = new GeometryShaderRenderWidget2;
    pLayout->addWidget(m_pRenderWidget2, 0, 1, 1, 1);

    m_pRenderWidget3 = new GeometryShaderRenderWidget3;
    pLayout->addWidget(m_pRenderWidget3, 1, 0, 1, 1);

    m_pRenderWidget4 = new GeometryShaderRenderWidget4;
    pLayout->addWidget(m_pRenderWidget4, 1, 1, 1, 1);
}

GeometryShaderWidget::~GeometryShaderWidget()
{

}

