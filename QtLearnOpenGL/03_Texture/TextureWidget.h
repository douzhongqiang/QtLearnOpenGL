#ifndef TEXTUREWIDGET_H
#define TEXTUREWIDGET_H

#include <QWidget>

class TextureRenderWidget;
class UICustomDoubleControl;
class TextureWidget : public QWidget
{
    Q_OBJECT

public:
    TextureWidget(QWidget* parent = nullptr);
    ~TextureWidget();

private:
    TextureRenderWidget* m_pRenderWidget = nullptr;
    UICustomDoubleControl* m_pMinNumberControl = nullptr;

private slots:
    void onValueChanged(qreal value, bool cmd);
};

#endif
