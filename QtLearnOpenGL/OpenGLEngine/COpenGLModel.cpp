#include "COpenGLModel.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include "OpenGLEngine/CAttributePoint.h"
#include "OpenGLEngine/COpenGLTexture.h"

COpenGLModel::COpenGLModel(QOpenGLFunctions* function, QOpenGLShaderProgram* pProgram, QObject* parent)
    :QObject(parent)
    , m_pFunction(function)
    , m_pShaderProgram(pProgram)
{

}

COpenGLModel::~COpenGLModel()
{

}

bool COpenGLModel::loadModel(const QString& path)
{
    QFile file(path);
    if (!file.exists())
        return false;

    QFileInfo info(path);
    m_dirPath = info.absolutePath();

    // 加载模型
    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFile(path.toLocal8Bit().data(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if (pScene == nullptr || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
    {
        qDebug() << "Load Model Error" << importer.GetErrorString();
        return false;
    }

    this->processNode(pScene->mRootNode, pScene);
    return true;
}

void COpenGLModel::processNode(aiNode* pNode, const aiScene* pScene)
{
    for (unsigned int i=0; i<pNode->mNumMeshes; ++i)
    {
        aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[i]];
        processMesh(pMesh, pScene);
    }

    for (unsigned int i=0; i<pNode->mNumChildren; ++i)
    {
        processNode(pNode->mChildren[i], pScene);
    }
}

void COpenGLModel::processMesh(aiMesh* pMesh, const aiScene* pScene)
{
    COpenGLMesh* pOpenglMesh = new COpenGLMesh(m_pFunction, m_pShaderProgram, this);
    m_meshs.append(pOpenglMesh);

    // 处理顶点、法线、纹理坐标信息
    CAttributePointArray pointArray;
    for (unsigned int i=0; i<pMesh->mNumVertices; ++i)
    {
        CAttributePoint attrPoint;

        // 设置点信息
        aiVector3D pVertices = pMesh->mVertices[i];
        attrPoint.pos[0] = pVertices.x;
        attrPoint.pos[1] = pVertices.y;
        attrPoint.pos[2] = pVertices.z;

        aiVector3D pNormal = pMesh->mNormals[i];
        attrPoint.normal[0] = pNormal.x;
        attrPoint.normal[1] = pNormal.y;
        attrPoint.normal[2] = pNormal.z;

        if (pMesh->mTextureCoords[0])
        {
            attrPoint.textureCoord[0] = pMesh->mTextureCoords[0][i].x;
            attrPoint.textureCoord[1] = pMesh->mTextureCoords[0][i].y;
        }
        else
        {
            attrPoint.textureCoord[0] = 0.0f;
            attrPoint.textureCoord[1] = 0.0f;
        }

        pointArray.append(attrPoint);
    }
    pOpenglMesh->setPoints(pointArray);

    // 设置索引
    QVector<unsigned int> indicesVec;
    for (unsigned int i =0; i<pMesh->mNumFaces; ++i)
    {
        aiFace face = pMesh->mFaces[i];
        for (unsigned int j=0; j<face.mNumIndices; ++j)
        {
            indicesVec << face.mIndices[j];
        }
    }
    pOpenglMesh->setIndices(indicesVec);

    // 加载材质
    int nMaterialIndex = pMesh->mMaterialIndex;
    if (nMaterialIndex >= 0)
    {
        aiMaterial* pMaterial = pScene->mMaterials[nMaterialIndex];
        processMaterial(pMaterial, aiTextureType_DIFFUSE, pOpenglMesh);
        processMaterial(pMaterial, aiTextureType_SPECULAR, pOpenglMesh);
    }

    pOpenglMesh->setupMesh();
}

void COpenGLModel::processMaterial(aiMaterial* pMaterial, aiTextureType type, COpenGLMesh* pOpenGLMaterial)
{
    unsigned int diffuseCount = pMaterial->GetTextureCount(type);
    for (unsigned int i=0; i<diffuseCount; ++i)
    {
        aiString textureString;
        pMaterial->GetTexture(type, i, &textureString);

        // 判断是否需要创建纹理
        QString texturePath = m_dirPath + "/" + QString(textureString.C_Str());
        qDebug() << type << texturePath;

        bool needCreate = true;
        for (int i=0; i<m_textures.size(); ++i)
        {
            if (texturePath == m_textures[i]->getImagePath())
            {
                needCreate = false;
                pOpenGLMaterial->addTexture(m_textures[i]);
                break;
            }
        }

        if (!needCreate)
            continue;

        COpenGLTexture* pTexture = new COpenGLTexture(m_pFunction, this);
        pTexture->create();
        pTexture->setImage(texturePath);
        if (type == aiTextureType_DIFFUSE)
            pTexture->setType(COpenGLTexture::t_diffuse);
        else
            pTexture->setType(COpenGLTexture::t_specular);
        m_textures.append(pTexture);
        pOpenGLMaterial->addTexture(pTexture);
    }
}

void COpenGLModel::draw(void)
{
    for (auto iter = m_meshs.begin(); iter != m_meshs.end(); ++iter)
    {
        (*iter)->draw();
    }
}
