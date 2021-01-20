#ifndef COPENGLELEMENTARRAY_H
#define COPENGLELEMENTARRAY_H

#include <QObject>
#include <QOpenGLFunctions>
#include <functional>

class COpenGLElementArray : public QObject
{
    Q_OBJECT

public:
    COpenGLElementArray(QOpenGLFunctions* function, QObject* parent = nullptr);
    ~COpenGLElementArray();

    // 创建
    void create(void* pData, int size);
    // 绑定
    void bind(void);
    void unbind(void);

    // 获取id
    GLuint getId(void);

    void setCallbackFunction(std::function<void(void)> func);

private:
    QOpenGLFunctions* m_pFunction = nullptr;

    GLuint m_id;    

    std::function<void(void)> m_fun;
};

#endif
