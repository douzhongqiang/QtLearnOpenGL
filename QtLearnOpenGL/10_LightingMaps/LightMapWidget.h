#ifndef LIGHTMAPWIDGET_H
#define LIGHTMAPWIDGET_H

#include <QWidget>

class LightMapRenderWidget;
class LightMapNode;

class LightMapWidget : public QWidget
{
    Q_OBJECT

public:
    LightMapWidget(QWidget* parent = nullptr);
    ~LightMapWidget();

private:
    LightMapRenderWidget* m_pRenderWidget = nullptr;
    LightMapNode* m_pMaterialNode = nullptr;

private slots:
    void onAttributeChanged(const QVariant& variant, bool isCmd);
};

#endif
