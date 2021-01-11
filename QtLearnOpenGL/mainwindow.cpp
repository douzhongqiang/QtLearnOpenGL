#include "mainwindow.h"
#include "UICurtomLeftBarControl/UICustomLefTabsControl.h"
#include "01_HelloTriangle/HelloTriangle.h"
#include "02_IBOAndPolygonMode/IBOAndPolygonMode.h"
#include "03_Texture/TextureWidget.h"
#include "04_Cube/CubeWidget.h"
#include "05_Cubes/CubesWidget.h"
#include "06_Camera/CameraWidget.h"
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
}

