#include "BlendTestWidget.h"
#include "BlendTestRenderWidget.h"
#include "BlendTestAttrNode.h"
#include "NDAttributeBase.h"
#include "UINodeAttrControl.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QFileDialog>

BlendTestWidget::BlendTestWidget(QWidget* parent)
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
    m_pDepthTestAttrNode = new BlendTestAttrNode(this);
    QWidget* pW = UINodeAttrControl::createNodeWidget(m_pDepthTestAttrNode);
    pW->setMaximumWidth(450);
    pSpliter->addWidget(pW);

    // 初始化节点
    m_pDepthTestAttrNode->setLightInfo(m_pRenderWidget->getLightInfo());
    m_pDepthTestAttrNode->setCameraPostion(QVector3D(0.0f, -1.0f, 7.0f));
    m_pDepthTestAttrNode->setCameraFront(QVector3D(0.0f, 0.0f, -1.0f));

    QObject::connect(m_pDepthTestAttrNode, &BlendTestAttrNode::attributeValueChanged, this, &BlendTestWidget::onAttributeChanged);
}

BlendTestWidget::~BlendTestWidget()
{

}

QWidget* BlendTestWidget::createRenderWidget(void)
{
    QWidget* pWidget = new QWidget;
    QVBoxLayout* pLayout = new QVBoxLayout(pWidget);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);

    // 添加渲染界面
    m_pRenderWidget = new BlendTestRenderWidget;
    QObject::connect(m_pRenderWidget, &BlendTestRenderWidget::attributeInfoChanged, this, &BlendTestWidget::onAttributeInfoChanged);
    m_pRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pLayout->addWidget(m_pRenderWidget);

    m_pButton = new QPushButton(tr("Show DepthTest"));
    pLayout->addWidget(m_pButton);
    QObject::connect(m_pButton, &QPushButton::clicked, this, &BlendTestWidget::onClickedDepthTest);

    return pWidget;
}

void BlendTestWidget::onClickedDepthTest(void)
{
    bool isVisible = m_pRenderWidget->isDepthTestVisible();
    if (isVisible)
        m_pButton->setText(tr("Show Scene"));
    else
        m_pButton->setText(tr("Show DepthTest"));

    m_pRenderWidget->setDepthTestVisible(!isVisible);
}

void BlendTestWidget::onAttributeChanged(const QVariant& variant, bool isCmd)
{
    m_pRenderWidget->setLightInfo(m_pDepthTestAttrNode->getLightInfo());

    m_pRenderWidget->setCameraPostion(m_pDepthTestAttrNode->getCameraPostion());
    m_pRenderWidget->setCameraFront(m_pDepthTestAttrNode->getCameraFront());
}

void BlendTestWidget::onAttributeInfoChanged(void)
{
    m_pDepthTestAttrNode->setLightInfo(m_pRenderWidget->getLightInfo());

    m_pDepthTestAttrNode->setCameraPostion(m_pRenderWidget->getCameraPostion());
    m_pDepthTestAttrNode->setCameraFront(m_pRenderWidget->getCameraFront());
}
