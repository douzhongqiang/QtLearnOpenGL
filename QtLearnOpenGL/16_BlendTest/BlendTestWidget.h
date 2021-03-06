#ifndef BLENDTESTWIDGET_H
#define BLENDTESTWIDGET_H

#include <QWidget>

class BlendTestRenderWidget;
class BlendTestAttrNode;
class QPushButton;

class BlendTestWidget : public QWidget
{
    Q_OBJECT

public:
    BlendTestWidget(QWidget* parent = nullptr);
    ~BlendTestWidget();

private:
    BlendTestRenderWidget* m_pRenderWidget = nullptr;
    BlendTestAttrNode* m_pDepthTestAttrNode = nullptr;

    QWidget* createRenderWidget(void);

private slots:
    void onAttributeChanged(const QVariant& variant, bool isCmd);
    void onAttributeInfoChanged(void);
};

#endif
