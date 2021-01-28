#include "DepthTestWidget.h"
#include "DepthTestRenderWidget.h"
#include "NDAttributeBase.h"
#include "UINodeAttrControl.h"
#include "DepthTestAttrNode.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QFileDialog>

DepthTestWidget::DepthTestWidget(QWidget* parent)
    :QWidget(parent)
{
    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->setMargin(2);
    pLayout->setSpacing(2);

    QSplitter* pSpliter = new QSplitter(Qt::Horizontal);
    pLayout->addWidget(pSpliter);

    // 添加渲染界面
    QWidget* pRenderWidget = createRenderWidget();
    pSpliter->addWidget(pRenderWidget);

    // 添加节点属性
    m_pDepthTestAttrNode = new DepthTestAttrNode(this);
    QWidget* pW = UINodeAttrControl::createNodeWidget(m_pDepthTestAttrNode);
    pW->setMaximumWidth(450);
    pSpliter->addWidget(pW);

    // 初始化节点
    m_pDepthTestAttrNode->setLightInfo(m_pRenderWidget->getLightInfo());
    m_pDepthTestAttrNode->setCameraPostion(QVector3D(0.0f, -1.0f, 7.0f));
    m_pDepthTestAttrNode->setCameraFront(QVector3D(0.0f, 0.0f, -1.0f));

    QObject::connect(m_pDepthTestAttrNode, &DepthTestAttrNode::attributeValueChanged, this, &DepthTestWidget::onAttributeChanged);
}

DepthTestWidget::~DepthTestWidget()
{

}

QWidget* DepthTestWidget::createRenderWidget(void)
{
    QWidget* pWidget = new QWidget;
    QVBoxLayout* pLayout = new QVBoxLayout(pWidget);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);

    // 添加渲染界面
    m_pRenderWidget = new DepthTestRenderWidget;
    QObject::connect(m_pRenderWidget, &DepthTestRenderWidget::attributeInfoChanged, this, &DepthTestWidget::onAttributeInfoChanged);
    m_pRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pLayout->addWidget(m_pRenderWidget);

    m_pButton = new QPushButton(tr("Show DepthTest"));
    pLayout->addWidget(m_pButton);
    QObject::connect(m_pButton, &QPushButton::clicked, this, &DepthTestWidget::onClickedDepthTest);

    return pWidget;
}

void DepthTestWidget::onClickedDepthTest(void)
{
    bool isVisible = m_pRenderWidget->isDepthTestVisible();
    if (isVisible)
        m_pButton->setText(tr("Show Scene"));
    else
        m_pButton->setText(tr("Show DepthTest"));

    m_pRenderWidget->setDepthTestVisible(!isVisible);
}

void DepthTestWidget::onAttributeChanged(const QVariant& variant, bool isCmd)
{
    m_pRenderWidget->setLightInfo(m_pDepthTestAttrNode->getLightInfo());

    m_pRenderWidget->setCameraPostion(m_pDepthTestAttrNode->getCameraPostion());
    m_pRenderWidget->setCameraFront(m_pDepthTestAttrNode->getCameraFront());
}

void DepthTestWidget::onAttributeInfoChanged(void)
{
    m_pDepthTestAttrNode->setLightInfo(m_pRenderWidget->getLightInfo());

    m_pDepthTestAttrNode->setCameraPostion(m_pRenderWidget->getCameraPostion());
    m_pDepthTestAttrNode->setCameraFront(m_pRenderWidget->getCameraFront());
}
