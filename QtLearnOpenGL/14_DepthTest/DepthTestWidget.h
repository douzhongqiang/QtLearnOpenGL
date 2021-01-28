#ifndef DEPTHTESTWIDGET_H
#define DEPTHTESTWIDGET_H

#include <QWidget>

class DepthTestRenderWidget;
class DepthTestAttrNode;
class QPushButton;
class DepthTestAttrNode;

class DepthTestWidget : public QWidget
{
    Q_OBJECT

public:
    DepthTestWidget(QWidget* parent = nullptr);
    ~DepthTestWidget();

private:
    DepthTestRenderWidget* m_pRenderWidget = nullptr;
    QPushButton* m_pButton = nullptr;
    DepthTestAttrNode* m_pDepthTestAttrNode = nullptr;

    QWidget* createRenderWidget(void);

private slots:
    void onClickedDepthTest(void);
    void onAttributeChanged(const QVariant& variant, bool isCmd);
    void onAttributeInfoChanged(void);
};

#endif
