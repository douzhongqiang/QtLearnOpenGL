#include "mainwindow.h"

#include <QApplication>
#include "UICustomCore/CustomStyleConfig.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    g_StyleConfig->init();

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
