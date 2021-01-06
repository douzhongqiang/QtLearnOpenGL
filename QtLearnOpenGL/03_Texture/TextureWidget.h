#ifndef TEXTUREWIDGET_H
#define TEXTUREWIDGET_H

#include <QWidget>

class TextureRenderWidget;
class TextureWidget : public QWidget
{
    Q_OBJECT

public:
    TextureWidget(QWidget* parent = nullptr);
    ~TextureWidget();

private:
    TextureRenderWidget* m_pRenderWidget = nullptr;

private slots:
    void onClickedButton(void);
};

#endif
