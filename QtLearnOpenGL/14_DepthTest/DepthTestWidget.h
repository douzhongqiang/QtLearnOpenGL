#ifndef DEPTHTESTWIDGET_H
#define DEPTHTESTWIDGET_H

#include <QWidget>

class DepthTestRenderWidget;
class DepthTestAttrNode;

class DepthTestWidget : public QWidget
{
    Q_OBJECT

public:
    DepthTestWidget(QWidget* parent = nullptr);
    ~DepthTestWidget();

private:
    DepthTestRenderWidget* m_pRenderWidget = nullptr;

    QWidget* createRenderWidget(void);
};

#endif
