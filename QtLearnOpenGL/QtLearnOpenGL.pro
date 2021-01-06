QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

LIBS += -L$$PWD/../bin -lCustomWidgets
LIBS += -L$$PWD/../bin -lCustomControls
LIBS += -L$$PWD/../bin -lUtils
LIBS += -L$$PWD/../bin -lNDNodeAttribute
LIBS += -L$$PWD/../bin -lNodeAttributeControl

INCLUDEPATH += $$PWD/../CustomWidgets
INCLUDEPATH += $$PWD/../CustomControls
INCLUDEPATH += $$PWD/../NDNodeAttribute
INCLUDEPATH += $$PWD/../NodeAttributeControl
INCLUDEPATH += $$PWD/../Utils

MOC_DIR += $$PWD/temp
OBJECTS_DIR += $$PWD/temp
DESTDIR += $$PWD/../bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    01_HelloTriangle/HelloTriangle.cpp \
    02_IBOAndPolygonMode/IBOAndPolygonMode.cpp \
    02_IBOAndPolygonMode/IBOAndPolygonModeWidget.cpp \
    03_Texture/TextureRenderWidget.cpp \
    03_Texture/TextureWidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    01_HelloTriangle/HelloTriangle.h \
    02_IBOAndPolygonMode/IBOAndPolygonMode.h \
    02_IBOAndPolygonMode/IBOAndPolygonModeWidget.h \
    03_Texture/TextureRenderWidget.h \
    03_Texture/TextureWidget.h \
    mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    shaders.qrc
