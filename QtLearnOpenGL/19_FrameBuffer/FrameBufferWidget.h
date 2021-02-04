#ifndef FRAMEBUFFERWIDGET_H
#define FRAMEBUFFERWIDGET_H

#include <QWidget>

class FrameBufferRenderWidget;
class FrameBufferAttrNode;
class QPushButton;

class FrameBufferWidget : public QWidget
{
    Q_OBJECT

public:
    FrameBufferWidget(QWidget* parent = nullptr);
    ~FrameBufferWidget();

private:
    FrameBufferRenderWidget* m_pRenderWidget = nullptr;
    QPushButton* m_pButton = nullptr;
    FrameBufferAttrNode* m_pDepthTestAttrNode = nullptr;

    QWidget* createRenderWidget(void);

private slots:
    void onAttributeChanged(const QVariant& variant, bool isCmd);
    void onAttributeInfoChanged(void);
};

#endif
