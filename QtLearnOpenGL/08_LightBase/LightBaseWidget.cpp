#include "LightBaseWidget.h"
#include "LightBaseRenderWidget.h"
#include "CustomCombineControl/UICustomVec3Control.h"
#include "CustomCombineControl/UICustomDoubleControl.h"
#include "CustomCombineControl/UICustomColorControl.h"
#include <QVBoxLayout>
#include <QPushButton>

LightBaseWidget::LightBaseWidget(QWidget* parent)
    :QWidget(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(2);
    pLayout->setSpacing(2);

    m_pRenderWidget = new LightBaseRenderWidget;
    m_pRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pLayout->addWidget(m_pRenderWidget);

    m_pObjectColorControl = new UICustomColorControl;
    m_pObjectColorControl->setColumnWidth(0, 230);
    m_pObjectColorControl->setCurrentColor(m_pRenderWidget->getObjectColor());
    m_pObjectColorControl->setTagText(tr("Object Color: "));
    pLayout->addWidget(m_pObjectColorControl);
    QObject::connect(m_pObjectColorControl, &UICustomColorControl::colorChanged, this, &LightBaseWidget::onObjectColorChanged);
    QObject::connect(m_pObjectColorControl, &UICustomColorControl::colorDragChanged, this, &LightBaseWidget::onObjectColorChanged);

    m_pLightColorControl = new UICustomColorControl;
    m_pLightColorControl->setColumnWidth(0, 230);
    m_pLightColorControl->setCurrentColor(m_pRenderWidget->getLightColor());
    m_pLightColorControl->setTagText(tr("Light Color: "));
    pLayout->addWidget(m_pLightColorControl);
    QObject::connect(m_pLightColorControl, &UICustomColorControl::colorChanged, this, &LightBaseWidget::onLightColorChanged);
    QObject::connect(m_pLightColorControl, &UICustomColorControl::colorDragChanged, this, &LightBaseWidget::onLightColorChanged);

    m_pLightPostionControl = new UICustomVec3Control;
    m_pLightPostionControl->setColumnWidth(0, 230);
    m_pLightPostionControl->setRange(-1000.0f, 1000.0f);
    m_pLightPostionControl->setValue(m_pRenderWidget->getLightPostion());
    m_pLightPostionControl->setTagText(tr("Light Postion: "));
    pLayout->addWidget(m_pLightPostionControl);
    QObject::connect(m_pLightPostionControl, &UICustomVec3Control::valueChanged, this, &LightBaseWidget::onLightPostionChanged);

}

LightBaseWidget::~LightBaseWidget()
{

}

void LightBaseWidget::onObjectColorChanged(const QColor& color, bool cmd)
{
    m_pRenderWidget->setObjectColor(color);
}

void LightBaseWidget::onLightColorChanged(const QColor& color, bool cmd)
{
    m_pRenderWidget->setLightColor(color);
}

void LightBaseWidget::onLightPostionChanged(const QVector3D& vec)
{
    m_pRenderWidget->setLightPostion(vec);
}
