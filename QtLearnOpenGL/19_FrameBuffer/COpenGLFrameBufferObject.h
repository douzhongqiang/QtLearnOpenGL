#ifndef COPENGLFRAMEBUFFEROBJECT_H
#define COPENGLFRAMEBUFFEROBJECT_H

#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QObject>

class COpenGLFrameBufferObject : public QObject
{
    Q_OBJECT

public:
    COpenGLFrameBufferObject(int width, int height, QObject* parent = nullptr);
    ~COpenGLFrameBufferObject();

    bool create(void);
    void bind(void);
    void unbind(void);
    void release(void);

    // 获取颜色缓冲区的Texture
    GLuint getTextureId(void);

    int width(void);
    int height(void);

private:
    GLuint m_frameBufferObjectId = 0;
    GLuint m_textureId = 0;
    GLuint m_rboId = 0;
    bool m_isSuccessed = false;

    int m_nWidth = 0;
    int m_nHeight = 0;
};

#endif
