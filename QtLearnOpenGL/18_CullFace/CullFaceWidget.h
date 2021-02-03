#ifndef CULLFACEWIDGET_H
#define CULLFACEWIDGET_H

#include <QWidget>

class CullFaceRenderWidget;
class CullFaceAttrNode;
class QPushButton;

class CullFaceWidget : public QWidget
{
    Q_OBJECT

public:
    CullFaceWidget(QWidget* parent = nullptr);
    ~CullFaceWidget();

private:
    CullFaceRenderWidget* m_pRenderWidget = nullptr;
    QPushButton* m_pButton = nullptr;
    CullFaceAttrNode* m_pDepthTestAttrNode = nullptr;

    QWidget* createRenderWidget(void);

private slots:
    void onAttributeChanged(const QVariant& variant, bool isCmd);
    void onAttributeInfoChanged(void);
};

#endif
