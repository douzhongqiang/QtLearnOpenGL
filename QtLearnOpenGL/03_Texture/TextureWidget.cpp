#include "TextureWidget.h"
#include "TextureRenderWidget.h"
#include <QVBoxLayout>
#include <QPushButton>

TextureWidget::TextureWidget(QWidget* parent)
    :QWidget(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(2);
    pLayout->setSpacing(2);

    m_pRenderWidget = new TextureRenderWidget;
    m_pRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pLayout->addWidget(m_pRenderWidget);

    QPushButton* pButton = new QPushButton(tr("OK"));
    QObject::connect(pButton, &QPushButton::clicked, this, &TextureWidget::onClickedButton);
    pButton->setMinimumHeight(40);
    pLayout->addWidget(pButton);
}

TextureWidget::~TextureWidget()
{

}

void TextureWidget::onClickedButton(void)
{
    bool isFilled = m_pRenderWidget->isFill();
    m_pRenderWidget->setFillStatus(!isFilled);
}
