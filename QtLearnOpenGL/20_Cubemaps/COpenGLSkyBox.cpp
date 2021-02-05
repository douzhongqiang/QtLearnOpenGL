#include "COpenGLSkyBox.h"

COpenGLSkyBox::COpenGLSkyBox(QObject* parent)
    :QObject(parent)
{

}

COpenGLSkyBox::~COpenGLSkyBox()
{

}

void COpenGLSkyBox::init(void)
{
    // 初始化shader
    initShaderProgram();
    // 初始化Mesh
    initMesh();
}

// 设置CubeTexture
void COpenGLSkyBox::setCubeTexture(COpenGLCubeTexture* pTexture)
{
    m_pCubeTexture = pTexture;
}

// 绘制天空盒
void COpenGLSkyBox::draw(void)
{
    m_pShaderProgram->bind();

    m_pShaderProgram->release();
}

bool COpenGLSkyBox::initShaderProgram(void)
{
    // 加载顶点着色器
    QString vertexShaderStr(":/14_DepthTest/shader/vertexshader.vsh");
    QOpenGLShader* pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << pVertexShader->log();
        pVertexShader->deleteLater();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/15_StencilTest/shader/lightfragmentShader.fsh");
    QOpenGLShader* pFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    result = pFragmentShader->compileSourceFile(fragmentShaderStr);
    if (!result)
    {
        qDebug() << pFragmentShader->log();
        pFragmentShader->deleteLater();
        pVertexShader->deleteLater();
        return false;
    }

    // 创建ShaderProgram
    m_pShaderProgram = new QOpenGLShaderProgram(this);
    m_pShaderProgram->addShader(pVertexShader);
    m_pShaderProgram->addShader(pFragmentShader);
    return m_pShaderProgram->link();
}

void COpenGLSkyBox::initMesh(void)
{
    m_pMesh = new COpenGLMesh(nullptr, m_pShaderProgram, this);
}
