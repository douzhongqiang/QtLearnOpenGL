#include "LightCasterWidget.h"
#include "DirectionLightRenderWidget.h"
#include "PointLightRenderWidget.h"
#include "SpotLightRenderWidget.h"
#include "FlashLightRenderWidget.h"
#include "LightCasterNode.h"
#include "NDAttributeBase.h"
#include "UINodeAttrControl.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>

LightCasterWidget::LightCasterWidget(QWidget* parent)
    :QWidget(parent)
{
    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->setMargin(2);
    pLayout->setSpacing(2);

    m_pMaterialNode = new LightCasterNode(this);

    QSplitter* pSpliter = new QSplitter(Qt::Horizontal);
    pLayout->addWidget(pSpliter);

    // 创建渲染的界面
    QWidget* pRenderWidget = createRenderWidget();
    pSpliter->addWidget(pRenderWidget);

    // 创建节点属性界面
    QWidget* pW = UINodeAttrControl::createNodeWidget(m_pMaterialNode);
    pW->setMaximumWidth(450);
    pSpliter->addWidget(pW);

    QObject::connect(m_pMaterialNode, &LightCasterNode::attributeValueChanged, this, &LightCasterWidget::onAttributeChanged);
    QObject::connect(m_pFlashlightWiget, &FlashLightRenderWidget::cameraInfoChanged, this, &LightCasterWidget::onCameraInfoChanged);
}

LightCasterWidget::~LightCasterWidget()
{

}

void LightCasterWidget::onAttributeChanged(const QVariant& variant, bool isCmd)
{
    m_pRenderWidget->setLightInfo(m_pMaterialNode->getLightInfo());
    m_pPointRenderWidget->setLightInfo(m_pMaterialNode->getPointLightInfo());
    m_pSpotRenderWidget->setLightInfo(m_pMaterialNode->getSpotLightInfo());
    m_pFlashlightWiget->setLightInfo(m_pMaterialNode->getFlashLightInfo());
}

void LightCasterWidget::onCameraInfoChanged(void)
{
    m_pMaterialNode->setFlashLightInfo(m_pFlashlightWiget->getLightInfo());
}

QWidget* LightCasterWidget::createRenderWidget(void)
{
    QWidget* pWidget = new QWidget;
    QGridLayout* pLayout = new QGridLayout(pWidget);
    pLayout->setMargin(0);
    pLayout->setSpacing(2);

    // 平行光
    m_pRenderWidget = new DirectionLightRenderWidget;
    m_pMaterialNode->setLightInfo(m_pRenderWidget->getLightInfo());
    m_pRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pLayout->addWidget(m_pRenderWidget, 0, 0);

    // 点光源
    m_pPointRenderWidget = new PointLightRenderWidget;
    m_pMaterialNode->setPointLightInfo(m_pPointRenderWidget->getLightInfo());
    m_pPointRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pLayout->addWidget(m_pPointRenderWidget, 0, 1);

    // 聚光灯
    m_pSpotRenderWidget = new SpotLightRenderWidget;
    m_pMaterialNode->setSpotLightInfo(m_pSpotRenderWidget->getLightInfo());
    m_pSpotRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pLayout->addWidget(m_pSpotRenderWidget, 1, 0);

    // 手电筒
    m_pFlashlightWiget = new FlashLightRenderWidget;
    m_pMaterialNode->setFlashLightInfo(m_pFlashlightWiget->getLightInfo());
    m_pFlashlightWiget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pLayout->addWidget(m_pFlashlightWiget, 1, 1);

    return pWidget;
}
