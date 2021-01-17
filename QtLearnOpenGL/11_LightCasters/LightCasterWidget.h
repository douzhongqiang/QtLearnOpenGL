#ifndef LIGHTCASTERWIDGET_H
#define LIGHTCASTERWIDGET_H

#include <QWidget>

class DirectionLightRenderWidget;
class LightCasterNode;
class PointLightRenderWidget;
class SpotLightRenderWidget;
class FlashLightRenderWidget;

class LightCasterWidget : public QWidget
{
    Q_OBJECT

public:
    LightCasterWidget(QWidget* parent = nullptr);
    ~LightCasterWidget();

private:
    DirectionLightRenderWidget* m_pRenderWidget = nullptr;
    PointLightRenderWidget* m_pPointRenderWidget = nullptr;
    SpotLightRenderWidget* m_pSpotRenderWidget = nullptr;
    FlashLightRenderWidget* m_pFlashlightWiget = nullptr;
    QWidget* createRenderWidget(void);

    LightCasterNode* m_pMaterialNode = nullptr;

private slots:
    void onAttributeChanged(const QVariant& variant, bool isCmd);
    void onCameraInfoChanged(void);
};

#endif
