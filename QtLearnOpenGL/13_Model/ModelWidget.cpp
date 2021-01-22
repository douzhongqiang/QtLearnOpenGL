#include "ModelWidget.h"
#include "ModelRenderWidget.h"
#include "ModelAttrNode.h"
#include "NDAttributeBase.h"
#include "UINodeAttrControl.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QFileDialog>

ModelWidget::ModelWidget(QWidget* parent)
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
    m_pMaterialNode = new ModelAttrNode(this);
    QWidget* pW = UINodeAttrControl::createNodeWidget(m_pMaterialNode);
    pW->setMaximumWidth(450);
    pSpliter->addWidget(pW);

    // 初始化节点
    m_pMaterialNode->setLightInfo(m_pRenderWidget->getLightInfo());

    QObject::connect(m_pMaterialNode, &ModelAttrNode::attributeValueChanged, this, &ModelWidget::onAttributeChanged);
}

ModelWidget::~ModelWidget()
{

}

QWidget* ModelWidget::createRenderWidget(void)
{
    QWidget* pWidget = new QWidget;
    QVBoxLayout* pLayout = new QVBoxLayout(pWidget);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);

    // 添加渲染界面
    m_pRenderWidget = new ModelRenderWidget;
    m_pRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pLayout->addWidget(m_pRenderWidget);

    // 添加按钮
    QPushButton* pButton = new QPushButton(tr("Load Model"));
    pLayout->addWidget(pButton);
    QObject::connect(pButton, &QPushButton::clicked, this, &ModelWidget::onClickedButton);
    return pWidget;
}

void ModelWidget::onAttributeChanged(const QVariant& variant, bool isCmd)
{
    m_pRenderWidget->setLightInfo(m_pMaterialNode->getLightInfo());
}

void ModelWidget::onClickedButton(void)
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select Model"), "./");
    m_pRenderWidget->setModel(filename);
}
