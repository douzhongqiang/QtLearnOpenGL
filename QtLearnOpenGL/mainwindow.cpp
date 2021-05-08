#include "mainwindow.h"
#include "UICurtomLeftBarControl/UICustomLefTabsControl.h"
#include "01_HelloTriangle/HelloTriangle.h"
#include "02_IBOAndPolygonMode/IBOAndPolygonMode.h"
#include "03_Texture/TextureWidget.h"
#include "04_Cube/CubeWidget.h"
#include "05_Cubes/CubesWidget.h"
#include "06_Camera/CameraWidget.h"
#include "07_CameraRoam/CameraRoamWidget.h"
#include "08_LightBase/LightBaseWidget.h"
#include "09_Material/MaterialWidget.h"
#include "10_LightingMaps/LightMapWidget.h"
#include "11_LightCasters/LightCasterWidget.h"
#include "13_Model/ModelWidget.h"
#include "14_DepthTest/DepthTestWidget.h"
#include "15_StencilTest/StencilTestWidget.h"
#include "16_BlendTest/BlendTestWidget.h"
#include "17_InvertedImage/InvertedImageWidget.h"
#include "18_CullFace/CullFaceWidget.h"
#include "19_FrameBuffer/FrameBufferWidget.h"
#include "20_Cubemaps/CubemapsWidget.h"
#include "21_GeometryShader/GeometryShaderWidget.h"
#include "22_DrawInstance/DrawInstanceWidget.h"
#include "23_TransformFeedback/TransformFeedbackRenderWidget.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* pCenterWidget = new QWidget;
    this->setCentralWidget(pCenterWidget);

    QVBoxLayout* pLayout = new QVBoxLayout(pCenterWidget);
    pLayout->setSpacing(0);
    pLayout->setMargin(2);

    m_pLeftControl = new UICustomLeftTabsControl;
    pLayout->addWidget(m_pLeftControl);

    initUI();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initUI(void)
{
    HelloTriangleWidget* pW1 = new HelloTriangleWidget;
    m_pLeftControl->addWidget(pW1, tr("01.Hello Triangle"));

    IBOAndPolygonMode* pW2 = new IBOAndPolygonMode;
    m_pLeftControl->addWidget(pW2, tr("02.IBO And PolygonMode"));

    TextureWidget* pW3 = new TextureWidget;
    m_pLeftControl->addWidget(pW3, tr("03.Texture"));

    CubeWidget* pW4 = new CubeWidget;
    m_pLeftControl->addWidget(pW4, tr("04.Cube"));

    CubesWidget* pW5 = new CubesWidget;
    m_pLeftControl->addWidget(pW5, tr("05.Cubes"));

    CameraWidget* pW6 = new CameraWidget;
    m_pLeftControl->addWidget(pW6, tr("06.Camera"));

    CameraRoamWidget* pW7 = new CameraRoamWidget;
    m_pLeftControl->addWidget(pW7, tr("07.CameraRoam"));

    LightBaseWidget* pW8 = new LightBaseWidget;
    m_pLeftControl->addWidget(pW8, tr("08.Light Base"));

    MaterialWidget* pW9 = new MaterialWidget;
    m_pLeftControl->addWidget(pW9, tr("09.Material"));

    LightMapWidget* pW10 = new LightMapWidget;
    m_pLeftControl->addWidget(pW10, tr("10.Lighting Maps"));

    LightCasterWidget* pW11 = new LightCasterWidget;
    m_pLeftControl->addWidget(pW11, tr("11.Lighting Caster"));

    ModelWidget* pW13 = new ModelWidget;
    m_pLeftControl->addWidget(pW13, tr("13.Model"));

    DepthTestWidget* pW14 = new DepthTestWidget;
    m_pLeftControl->addWidget(pW14, tr("14.DepthTest"));

    StencilTestWidget* pW15 = new StencilTestWidget;
    m_pLeftControl->addWidget(pW15, tr("15.StencilTest"));

    BlendTestWidget* pW16 = new BlendTestWidget;
    m_pLeftControl->addWidget(pW16, tr("16.BlendTest"));

    InvertedImageWidget* pW17 = new InvertedImageWidget;
    m_pLeftControl->addWidget(pW17, tr("17.Inverted Image"));

    CullFaceWidget* pW18 = new CullFaceWidget;
    m_pLeftControl->addWidget(pW18, tr("18.Cull Face"));

    FrameBufferWidget* pW19 = new FrameBufferWidget;
    m_pLeftControl->addWidget(pW19, tr("19.Frame Buffer"));

    CubemapsWidget* pW20 = new CubemapsWidget;
    m_pLeftControl->addWidget(pW20, tr("20.Cubemaps"));

    GeometryShaderWidget* pW21 = new GeometryShaderWidget;
    m_pLeftControl->addWidget(pW21, tr("21.Geometry Shader"));

    DrawInstanceWidget* pW22 = new DrawInstanceWidget;
    m_pLeftControl->addWidget(pW22, tr("22.Draw Instance"));

    TransformFeedbackRenderWidget* pW23 = new TransformFeedbackRenderWidget;
    m_pLeftControl->addWidget(pW23, tr("23. Transform Feedback"));
}
