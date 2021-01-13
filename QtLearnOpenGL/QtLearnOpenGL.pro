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
    04_Cube/CubeRenderWidget.cpp \
    04_Cube/CubeWidget.cpp \
    05_Cubes/CubesRenderWidget.cpp \
    05_Cubes/CubesWidget.cpp \
    06_Camera/CameraRenderWidget.cpp \
    06_Camera/CameraWidget.cpp \
    07_CameraRoam/CameraRoamRenderWidget.cpp \
    07_CameraRoam/CameraRoamWidget.cpp \
    08_LightBase/LightBaseRenderWidget.cpp \
    08_LightBase/LightBaseWidget.cpp \
    OpenGLEngine/CAttributePoint.cpp \
    OpenGLEngine/COpenGLCamera.cpp \
    OpenGLEngine/COpenGLRender.cpp \
    OpenGLEngine/COpenGLTexture.cpp \
    OpenGLEngine/COpenGLVertexArray.cpp \
    OpenGLEngine/COpenGLVertexObject.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    01_HelloTriangle/HelloTriangle.h \
    02_IBOAndPolygonMode/IBOAndPolygonMode.h \
    02_IBOAndPolygonMode/IBOAndPolygonModeWidget.h \
    03_Texture/TextureRenderWidget.h \
    03_Texture/TextureWidget.h \
    04_Cube/CubeRenderWidget.h \
    04_Cube/CubeWidget.h \
    05_Cubes/CubesRenderWidget.h \
    05_Cubes/CubesWidget.h \
    06_Camera/CameraRenderWidget.h \
    06_Camera/CameraWidget.h \
    07_CameraRoam/CameraRoamRenderWidget.h \
    07_CameraRoam/CameraRoamWidget.h \
    08_LightBase/LightBaseRenderWidget.h \
    08_LightBase/LightBaseWidget.h \
    OpenGLEngine/CAttributePoint.h \
    OpenGLEngine/COpenGLCamera.h \
    OpenGLEngine/COpenGLRender.h \
    OpenGLEngine/COpenGLTexture.h \
    OpenGLEngine/COpenGLVertexArray.h \
    OpenGLEngine/COpenGLVertexObject.h \
    mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    shaders.qrc
