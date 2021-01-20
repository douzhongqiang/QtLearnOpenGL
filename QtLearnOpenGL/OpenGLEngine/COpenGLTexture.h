#ifndef COPENGLTEXTURE_H
#define COPENGLTEXTURE_H

#include <QObject>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions>

class COpenGLTexture : public QObject
{
    Q_OBJECT

public:
    enum TextureType
    {
        t_diffuse,
        t_specular
    };

public:
    COpenGLTexture(QOpenGLFunctions* function, QObject* parent = nullptr);
    ~COpenGLTexture();

    // 创建纹理
    void create(void);
    // 绑定
    void bind(void);
    void unbind(void);
    // 设置图片
    void setImage(const QImage& image);
    void setImage(const QString& image);

    // 设置/获取纹理的名字 - shader中的名字
    void setName(const QString& name);
    QString getName(void);

    void setType(TextureType type);
    TextureType getType(void);

    // 获取ID
    GLuint getId(void);

    // 获取图片的路径
    QString getImagePath(void);

    void activeTexture(int textureId);

private:
    QOpenGLFunctions* m_pFunction = nullptr;
    GLuint m_id;
    QString m_name;
    QString m_imagePath;

    TextureType m_type;
};

#endif
