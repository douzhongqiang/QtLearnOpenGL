#include "LightMapWidget.h"
#include "LightMapRenderWidget.h"
#include "LightMapNode.h"
#include "NDAttributeBase.h"
#include "UINodeAttrControl.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>

LightMapWidget::LightMapWidget(QWidget* parent)
    :QWidget(parent)
{
    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->setMargin(2);
    pLayout->setSpacing(2);

    QSplitter* pSpliter = new QSplitter(Qt::Horizontal);
    pLayout->addWidget(pSpliter);

    m_pRenderWidget = new LightMapRenderWidget;
    m_pRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pSpliter->addWidget(m_pRenderWidget);

    m_pMaterialNode = new LightMapNode(this);
    QWidget* pW = UINodeAttrControl::createNodeWidget(m_pMaterialNode);
    pW->setMaximumWidth(450);
    pSpliter->addWidget(pW);

    // 初始化节点
    m_pMaterialNode->setObjectMaterial(m_pRenderWidget->getObjectMaterial());
    m_pMaterialNode->setLightInfo(m_pRenderWidget->getLightInfo());

    QObject::connect(m_pMaterialNode, &LightMapNode::attributeValueChanged, this, &LightMapWidget::onAttributeChanged);
}

LightMapWidget::~LightMapWidget()
{

}

void LightMapWidget::onAttributeChanged(const QVariant& variant, bool isCmd)
{
    m_pRenderWidget->setObjectMaterial(m_pMaterialNode->getObjectMaterial());
    m_pRenderWidget->setLightInfo(m_pMaterialNode->getLightInfo());
}
