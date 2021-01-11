#ifndef CUBESWIDGET_H
#define CUBESWIDGET_H

#include <QWidget>

class CubesRenderWidget;
class UICustomDoubleControl;
class UICustomVec3Control;
class UICustomIntControl;
class CubesWidget : public QWidget
{
    Q_OBJECT

public:
    CubesWidget(QWidget* parent = nullptr);
    ~CubesWidget();

private:
    CubesRenderWidget* m_pRenderWidget = nullptr;
};

#endif
