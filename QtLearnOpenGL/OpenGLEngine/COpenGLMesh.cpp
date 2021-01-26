#include "COpenGLMesh.h"

COpenGLMesh::COpenGLMesh(QOpenGLFunctions* function, QOpenGLShaderProgram* pProgram, QObject* parent)
    :QObject(parent)
    , m_pFunction(function)
    , m_pShaderProgram(pProgram)
{
    m_pObject = new COpenGLVertexObject(function, this);
    m_pObject->setProgram(pProgram);
    m_pObject->setType(COpenGLVertexObject::t_Triangle);
    m_pObject->setName("pos", "texCoord", "normal");
}

COpenGLMesh::~COpenGLMesh()
{

}

void COpenGLMesh::setPoints(const CAttributePointArray& points)
{
    m_pObject->setPoints(points);
}

void COpenGLMesh::setIndices(const QVector<unsigned int> indices)
{
    m_pObject->setIndices(indices);
}

void COpenGLMesh::addTexture(COpenGLTexture* texture)
{
    m_textures.append(texture);
}

void COpenGLMesh::setupMesh(void)
{
    QOpenGLFunctions_3_3_Core* pFunc = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    pFunc->initializeOpenGLFunctions();
    pFunc->glGenVertexArrays(1, &m_VAO);
    pFunc->glBindVertexArray(m_VAO);
    m_pObject->create();
    pFunc->glBindVertexArray(0);

    // 设置纹理
    int diffuseIndex = 0;
    int specularIndex = 0;
    for (auto iter = m_textures.begin(); iter != m_textures.end(); ++iter)
    {
        QString name = "objectMaterial.";
        if ((*iter)->getType() == COpenGLTexture::t_diffuse)
        {
            name += QString("diffuse")/* + QString::number(diffuseIndex++)*/;
            (*iter)->setName(name);
        }
        else if ((*iter)->getType() == COpenGLTexture::t_specular)
        {
            name += QString("specular")/* + QString::number(specularIndex++)*/;
            (*iter)->setName(name);
        }
    }
}

void COpenGLMesh::activeTexture(void)
{
    int count = m_textures.size();
    for (int i=0; i<count; ++i)
    {
        // 设置i号纹理
        m_pFunction->glActiveTexture(GL_TEXTURE0 + i);
        m_pFunction->glBindTexture(GL_TEXTURE_2D, m_textures[i]->getId());

        QString textureName = m_textures[i]->getName();
        m_pShaderProgram->setUniformValue(textureName.toLocal8Bit().data(), i);
    }
}

void COpenGLMesh::draw(void)
{
    activeTexture();

    QOpenGLFunctions_3_3_Core* pFunc = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    // 绘制
    pFunc->glBindVertexArray(m_VAO);
    m_pObject->renderSelf(true);
    pFunc->glBindVertexArray(0);

//    m_pFunction->glActiveTexture(GL_TEXTURE0);
}

void COpenGLMesh::setObjectType(COpenGLVertexObject::ObjectType type)
{
    m_pObject->setType(type);
}
