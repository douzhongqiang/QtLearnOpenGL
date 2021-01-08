#ifndef CUBEWIDGET_H
#define CUBEWIDGET_H

#include <QWidget>

class CubeRenderWidget;
class UICustomDoubleControl;
class CubeWidget : public QWidget
{
    Q_OBJECT

public:
    CubeWidget(QWidget* parent = nullptr);
    ~CubeWidget();

private:
    CubeRenderWidget* m_pRenderWidget = nullptr;
    UICustomDoubleControl* m_pMinNumberControl = nullptr;

private slots:
    void onValueChanged(qreal value, bool cmd);
};

#endif
