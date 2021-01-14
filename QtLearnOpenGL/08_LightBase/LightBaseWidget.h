#ifndef LIGHTBASEWIDGET_H
#define LIGHTBASEWIDGET_H

#include <QWidget>

class LightBaseRenderWidget;
class UICustomVec3Control;
class UICustomDoubleControl;
class UICustomColorControl;

class LightBaseWidget : public QWidget
{
    Q_OBJECT

public:
    LightBaseWidget(QWidget* parent = nullptr);
    ~LightBaseWidget();

private:
    LightBaseRenderWidget* m_pRenderWidget = nullptr;

    UICustomVec3Control* m_pLightPostionControl = nullptr;
    UICustomColorControl* m_pLightColorControl = nullptr;
    UICustomColorControl* m_pObjectColorControl = nullptr;

private slots:
    void onObjectColorChanged(const QColor& color, bool cmd);
    void onLightColorChanged(const QColor& color, bool cmd);
    void onLightPostionChanged(const QVector3D& vec);
};

#endif
