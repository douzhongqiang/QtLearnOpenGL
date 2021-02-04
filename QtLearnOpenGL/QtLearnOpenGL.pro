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

INCLUDEPATH += $$PWD/../ThirdParty/assimp/include
CONFIG(debug, debug|release) {
    #VS2019
    LIBS += -L$$PWD/../ThirdParty/assimp/lib/Debug -lassimp-vc140-mt
}
else{
    LIBS += -L$$PWD/../ThirdParty/assimp/lib/Release -lassimp-vc140-mt
}

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
    09_Material/MaterialNode.cpp \
    09_Material/MaterialRenderWidget.cpp \
    09_Material/MaterialWidget.cpp \
    10_LightingMaps/LightMapNode.cpp \
    10_LightingMaps/LightMapRenderWidget.cpp \
    10_LightingMaps/LightMapWidget.cpp \
    11_LightCasters/FlashLightRenderWidget.cpp \
    11_LightCasters/LightCasterNode.cpp \
    11_LightCasters/DirectionLightRenderWidget.cpp \
    11_LightCasters/LightCasterWidget.cpp \
    11_LightCasters/PointLightRenderWidget.cpp \
    11_LightCasters/SpotLightRenderWidget.cpp \
    13_Model/ModelAttrNode.cpp \
    13_Model/ModelRenderWidget.cpp \
    13_Model/ModelWidget.cpp \
    14_DepthTest/DepthTestAttrNode.cpp \
    14_DepthTest/DepthTestRenderWidget.cpp \
    14_DepthTest/DepthTestWidget.cpp \
    15_StencilTest/StencilTestAttrNode.cpp \
    15_StencilTest/StencilTestRenderWidget.cpp \
    15_StencilTest/StencilTestWidget.cpp \
    16_BlendTest/BlendTestAttrNode.cpp \
    16_BlendTest/BlendTestRenderWidget.cpp \
    16_BlendTest/BlendTestWidget.cpp \
    17_InvertedImage/InvertedImageAttrNode.cpp \
    17_InvertedImage/InvertedImageRenderWidget.cpp \
    17_InvertedImage/InvertedImageWidget.cpp \
    18_CullFace/CullFaceAttrNode.cpp \
    18_CullFace/CullFaceRenderWidget.cpp \
    18_CullFace/CullFaceWidget.cpp \
    19_FrameBuffer/COpenGLFrameBufferObject.cpp \
    19_FrameBuffer/FrameBufferAttrNode.cpp \
    19_FrameBuffer/FrameBufferRenderWidget.cpp \
    19_FrameBuffer/FrameBufferWidget.cpp \
    OpenGLEngine/CAttributePoint.cpp \
    OpenGLEngine/COpenGLCamera.cpp \
    OpenGLEngine/COpenGLElementArray.cpp \
    OpenGLEngine/COpenGLMesh.cpp \
    OpenGLEngine/COpenGLModel.cpp \
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
    09_Material/MaterialNode.h \
    09_Material/MaterialRenderWidget.h \
    09_Material/MaterialWidget.h \
    10_LightingMaps/LightMapNode.h \
    10_LightingMaps/LightMapRenderWidget.h \
    10_LightingMaps/LightMapWidget.h \
    11_LightCasters/FlashLightRenderWidget.h \
    11_LightCasters/LightCasterNode.h \
    11_LightCasters/DirectionLightRenderWidget.h \
    11_LightCasters/LightCasterWidget.h \
    11_LightCasters/PointLightRenderWidget.h \
    11_LightCasters/SpotLightRenderWidget.h \
    13_Model/ModelAttrNode.h \
    13_Model/ModelRenderWidget.h \
    13_Model/ModelWidget.h \
    14_DepthTest/DepthTestAttrNode.h \
    14_DepthTest/DepthTestRenderWidget.h \
    14_DepthTest/DepthTestWidget.h \
    15_StencilTest/StencilTestAttrNode.h \
    15_StencilTest/StencilTestRenderWidget.h \
    15_StencilTest/StencilTestWidget.h \
    16_BlendTest/BlendTestAttrNode.h \
    16_BlendTest/BlendTestRenderWidget.h \
    16_BlendTest/BlendTestWidget.h \
    17_InvertedImage/InvertedImageAttrNode.h \
    17_InvertedImage/InvertedImageRenderWidget.h \
    17_InvertedImage/InvertedImageWidget.h \
    18_CullFace/CullFaceAttrNode.h \
    18_CullFace/CullFaceRenderWidget.h \
    18_CullFace/CullFaceWidget.h \
    19_FrameBuffer/COpenGLFrameBufferObject.h \
    19_FrameBuffer/FrameBufferAttrNode.h \
    19_FrameBuffer/FrameBufferRenderWidget.h \
    19_FrameBuffer/FrameBufferWidget.h \
    OpenGLEngine/CAttributePoint.h \
    OpenGLEngine/COpenGLCamera.h \
    OpenGLEngine/COpenGLElementArray.h \
    OpenGLEngine/COpenGLMesh.h \
    OpenGLEngine/COpenGLModel.h \
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
