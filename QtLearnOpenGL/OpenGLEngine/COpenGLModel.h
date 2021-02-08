#ifndef COPENGLMODEL_H
#define COPENGLMODEL_H

#include <QObject>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "COpenGLMesh.h"
#include "OpenGLEngine/COpenGLTexture.h"

class COpenGLModel : public QObject
{
    Q_OBJECT

public:
    COpenGLModel(QOpenGLFunctions* function, QOpenGLShaderProgram* pProgram, QObject* parent = nullptr);
    ~COpenGLModel();

    bool loadModel(const QString& path);
    void processNode(aiNode* pNode, const aiScene* pScene);
    void processMesh(aiMesh* pMesh, const aiScene* pScene);
    void processMaterial(aiMaterial* pMaterial, aiTextureType type, COpenGLMesh* pOpenGLMaterial);

    void draw(void);
    void setTextureRenderEnabled(bool isEnabled);

private:
    QString m_dirPath;

    QVector<COpenGLMesh*> m_meshs;
    QVector<COpenGLTexture*> m_textures;

    QOpenGLFunctions* m_pFunction = nullptr;
    QOpenGLShaderProgram* m_pShaderProgram = nullptr;

    bool m_isTextureRender = true;
};

#endif
