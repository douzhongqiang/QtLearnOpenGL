#ifndef COPENGLVERTEXARRAY_H
#define COPENGLVERTEXARRAY_H

#include <QObject>
#include <QOpenGLFunctions>

class COpenGLVertexArray : public QObject
{
    Q_OBJECT

public:
    COpenGLVertexArray(QOpenGLFunctions* function, QObject* parent = nullptr);
    ~COpenGLVertexArray();

    // 创建
    void create(void* pData, int size);
    // 绑定
    void bind(void);
    void unbind(void);

    // 获取id
    GLuint getId(void);

private:
    QOpenGLFunctions* m_pFunction = nullptr;

    GLuint m_id;
};

#endif
