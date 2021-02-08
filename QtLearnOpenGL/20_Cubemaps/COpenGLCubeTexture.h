#ifndef COPENGLCUBETEXTURE_H
#define COPENGLCUBETEXTURE_H

#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QObject>

class COpenGLCubeTexture : public QObject
{
    Q_OBJECT

public:
    enum CubeTextureType
    {
        t_CubeRight,
        t_CubeLeft,
        t_CubeTop,
        t_CubeButtom,
        t_CubeBack,
        t_CubeFront
    };

public:
    COpenGLCubeTexture(QObject* parent = nullptr);
    ~COpenGLCubeTexture();

    void create(void);
    void bind(void);
    void unbind(void);

    void setImage(CubeTextureType type, const QImage& image);
    GLuint getTextureId(void);

private:
    QMap<CubeTextureType, QImage> m_cubeTextureMap;
    GLuint textureId = 0;
};

#endif
