#ifndef STENCILTESTWIDGET_H
#define STENCILTESTWIDGET_H

#include <QWidget>

class StencilTestRenderWidget;
class StencilTestAttrNode;
class QPushButton;

class StencilTestWidget : public QWidget
{
    Q_OBJECT

public:
    StencilTestWidget(QWidget* parent = nullptr);
    ~StencilTestWidget();

private:
    StencilTestRenderWidget* m_pRenderWidget = nullptr;
    QPushButton* m_pButton = nullptr;
    StencilTestAttrNode* m_pDepthTestAttrNode = nullptr;

    QWidget* createRenderWidget(void);

private slots:
    void onClickedDepthTest(void);
    void onAttributeChanged(const QVariant& variant, bool isCmd);
    void onAttributeInfoChanged(void);
};

#endif
