#include "CullFaceWidget.h"
#include "CullFaceRenderWidget.h"
#include "CullFaceAttrNode.h"
#include "NDAttributeBase.h"
#include "UINodeAttrControl.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QFileDialog>

CullFaceWidget::CullFaceWidget(QWidget* parent)
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
    m_pDepthTestAttrNode = new CullFaceAttrNode(this);
    QWidget* pW = UINodeAttrControl::createNodeWidget(m_pDepthTestAttrNode);
    pW->setMaximumWidth(450);
    pSpliter->addWidget(pW);

    // 初始化节点
    m_pDepthTestAttrNode->setLightInfo(m_pRenderWidget->getLightInfo());
    m_pDepthTestAttrNode->setCameraPostion(QVector3D(0.0f, -0.5f, 7.0f));
    m_pDepthTestAttrNode->setCameraFront(QVector3D(0.0f, 0.0f, -1.0f));

    QObject::connect(m_pDepthTestAttrNode, &CullFaceAttrNode::attributeValueChanged, this, &CullFaceWidget::onAttributeChanged);
}

CullFaceWidget::~CullFaceWidget()
{

}

QWidget* CullFaceWidget::createRenderWidget(void)
{
    QWidget* pWidget = new QWidget;
    QVBoxLayout* pLayout = new QVBoxLayout(pWidget);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);

    // 添加渲染界面
    m_pRenderWidget = new CullFaceRenderWidget;
    QObject::connect(m_pRenderWidget, &CullFaceRenderWidget::attributeInfoChanged, this, &CullFaceWidget::onAttributeInfoChanged);
    m_pRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pLayout->addWidget(m_pRenderWidget);

    return pWidget;
}

void CullFaceWidget::onAttributeChanged(const QVariant& variant, bool isCmd)
{
    m_pRenderWidget->setLightInfo(m_pDepthTestAttrNode->getLightInfo());

    m_pRenderWidget->setCameraPostion(m_pDepthTestAttrNode->getCameraPostion());
    m_pRenderWidget->setCameraFront(m_pDepthTestAttrNode->getCameraFront());

    // 设置面剔除相关设置
    m_pRenderWidget->setCullFaceEnabled(m_pDepthTestAttrNode->isCullFaceEnabled());
    m_pRenderWidget->setCullFaceType(m_pDepthTestAttrNode->getCullFaceType());
    m_pRenderWidget->setFrontOrderType(m_pDepthTestAttrNode->getFrontOrderType());
}

void CullFaceWidget::onAttributeInfoChanged(void)
{
    m_pDepthTestAttrNode->setLightInfo(m_pRenderWidget->getLightInfo());

    m_pDepthTestAttrNode->setCameraPostion(m_pRenderWidget->getCameraPostion());
    m_pDepthTestAttrNode->setCameraFront(m_pRenderWidget->getCameraFront());

    // 设置面剔除相关设置
    m_pDepthTestAttrNode->setCullFaceEnabled(m_pRenderWidget->isCullFaceEnabled());
    m_pDepthTestAttrNode->setCullFaceType(m_pRenderWidget->getCullFaceType());
    m_pDepthTestAttrNode->setFrontOrderType(m_pRenderWidget->getFrontOrderType());
}
