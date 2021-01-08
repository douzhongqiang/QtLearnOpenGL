#include "CubeRenderWidget.h"

#include <QDebug>

CubeRenderWidget::CubeRenderWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{

}

CubeRenderWidget::~CubeRenderWidget()
{
    qDebug() << __FUNCTION__;
}

void CubeRenderWidget::initializeGL()
{
    this->initializeOpenGLFunctions();

    // 初始化GPU程序
    bool result = initShaderProgram();
    if (!result)
        return;

    glEnable(GL_DEPTH_TEST);

    m_shaderProgramId = m_pShaderProgram->programId();
    // 获取位置和颜色的locationID
    m_nPosAttrLocationId = glGetAttribLocation(m_shaderProgramId, "pos");
    m_nCoordAttrLocationId = glGetAttribLocation(m_shaderProgramId, "texCoord");
    m_nNormalAttrLocationId = glGetAttribLocation(m_shaderProgramId, "normal");
    // 获取Uniform的Location
    m_nCoordLocationId = glGetUniformLocation(m_shaderProgramId, "texture1");
    m_nCoordLocationId2 = glGetUniformLocation(m_shaderProgramId, "texture2");
    m_nMixNumLocationId = glGetUniformLocation(m_shaderProgramId, "mixNumber");
    // MVP
    m_nMLocationId = glGetUniformLocation(m_shaderProgramId, "M");
    m_nVLocationId = glGetUniformLocation(m_shaderProgramId, "V");
    m_nPLocationId = glGetUniformLocation(m_shaderProgramId, "P");
    m_MMat.translate(QVector3D(0, 0, -5));
    m_PMat.perspective(45.0f, this->width() * 1.0 / this->height(), 1.0f, 10.0f);

    // 创建顶点属性数据
//    VertexAttributeData vAttrData[4];
//    createVertexAttributeData(vAttrData);
    initModelData();

    // 创建IBO数据
    QVector<GLuint> indices;
    for (int i=0; i<m_cube.size(); ++i)
        indices << i;

    // 创建VBO
    glGenBuffers(1, &m_nVBOId);
    // 创建IBO
    glGenBuffers(1, &m_nIBOId);

    // 初始化VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_nVBOId);
    qDebug() << sizeof(VertexAttributeData);
    glBufferData(GL_ARRAY_BUFFER, m_cube.size() * sizeof(VertexAttributeData), m_cube.data(), GL_STATIC_DRAW);

    // 初始化IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIBOId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // 设置顶点信息属性指针 - 3
    glVertexAttribPointer(m_nPosAttrLocationId, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributeData), (void*)0);
    glEnableVertexAttribArray(m_nPosAttrLocationId);
    // 设置原色信息属性指针 - 2
    glVertexAttribPointer(m_nCoordAttrLocationId, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttributeData), (void*)(sizeof (float) * 3));
    glEnableVertexAttribArray(m_nCoordAttrLocationId);
    // 设置纹理坐标 - 3
    glVertexAttribPointer(m_nNormalAttrLocationId, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributeData), (void*)(sizeof (float) * 5));
    glEnableVertexAttribArray(m_nNormalAttrLocationId);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 创建第一个纹理
    QImage image(":/03_Texture/image/TestImage1.jpg");
    image = image.convertToFormat(QImage::Format_RGB888);
    createTexture(image, m_nTextureId);

    // 创建第二个纹理
    QImage image2(":/03_Texture/image/TestImage2.jpg");
    image2 = image2.convertToFormat(QImage::Format_RGB888);
    createTexture(image2, m_nTextureId2);

    // 初始化定时器
    initTimer();
}

void CubeRenderWidget::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h);

    m_PMat.setToIdentity();
    m_PMat.perspective(45.0f, this->width() * 1.0 / this->height(), 1.0f, 10.0f);
    this->update();

    return QOpenGLWidget::resizeGL(w, h);
}

void CubeRenderWidget::paintGL()
{
    glClearColor(51.0f / 255.0f, 76.0f / 255.0f, 76.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_isFill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 使用shader
    m_pShaderProgram->bind();

    // 设置0号纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_nTextureId);
    glUniform1i(m_nCoordLocationId, 0);
    // 设置1号纹理
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_nTextureId2);
    glUniform1i(m_nCoordLocationId2, 1);
    // 设置混色的参数
    glUniform1f(m_nMixNumLocationId, m_mixNumber);

    // 设置矩阵
    glUniformMatrix4fv(m_nMLocationId, 1, GL_FALSE, m_MMat.data());
    glUniformMatrix4fv(m_nVLocationId, 1, GL_FALSE, m_VMat.data());
    glUniformMatrix4fv(m_nPLocationId, 1, GL_FALSE, m_PMat.data());

//    glBindBuffer(GL_ARRAY_BUFFER, m_nVBOId);
//    // 绘制
//    glDrawArrays(GL_QUADS, 0, m_cube.size());
//    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIBOId);
    // 绘制
    glDrawElements(GL_QUADS, m_cube.size(), GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    m_pShaderProgram->release();
}

bool CubeRenderWidget::initShaderProgram(void)
{
    m_pShaderProgram = new QOpenGLShaderProgram(this);

    // 加载顶点着色器
    QString vertexShaderStr(":/04_Cube/shader/vertexshader.vsh");
    m_pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = m_pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        qDebug() << m_pVertexShader->log();
        return false;
    }

    // 加载片段着色器
    QString fragmentShaderStr(":/04_Cube/shader/fragmentshader.fsh");
    m_pFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    result = m_pFragmentShader->compileSourceFile(fragmentShaderStr);
    if (!result)
    {
        qDebug() << m_pFragmentShader->log();
        return false;
    }

    // 创建ShaderProgram
    m_pShaderProgram = new QOpenGLShaderProgram(this);
    m_pShaderProgram->addShader(m_pVertexShader);
    m_pShaderProgram->addShader(m_pFragmentShader);
    return m_pShaderProgram->link();
}

//void CubeRenderWidget::createVertexAttributeData(VertexAttributeData* pVetAttr)
//{
//    // 第一个点位置信息
//    pVetAttr[0].pos[0] = -0.5f;
//    pVetAttr[0].pos[1] = 0.5f;
//    pVetAttr[0].pos[2] = 0.0f;
//    // 第一个点颜色信息
//    pVetAttr[0].color[0] = 1.0f;
//    pVetAttr[0].color[1] = 0.0f;
//    pVetAttr[0].color[2] = 0.0f;
//    // 第一个点的纹理坐标
//    pVetAttr[0].coord[0] = 0.0f;
//    pVetAttr[0].coord[1] = 1.0f;

//    // 第二个点位置信息
//    pVetAttr[1].pos[0] = -0.5f;
//    pVetAttr[1].pos[1] = -0.5f;
//    pVetAttr[1].pos[2] = 0.0f;
//    // 第二个点颜色信息
//    pVetAttr[1].color[0] = 0.0f;
//    pVetAttr[1].color[1] = 1.0f;
//    pVetAttr[1].color[2] = 0.0f;
//    // 第二个点的纹理坐标
//    pVetAttr[1].coord[0] = 0.0f;
//    pVetAttr[1].coord[1] = 0.0f;

//    // 第三个点位置信息
//    pVetAttr[2].pos[0] = 0.5f;
//    pVetAttr[2].pos[1] = -0.5f;
//    pVetAttr[2].pos[2] = 0.0f;
//    // 第三个点颜色信息
//    pVetAttr[2].color[0] = 0.0f;
//    pVetAttr[2].color[1] = 0.0f;
//    pVetAttr[2].color[2] = 1.0f;
//    // 第三个点的纹理坐标
//    pVetAttr[2].coord[0] = 1.0f;
//    pVetAttr[2].coord[1] = 0.0f;

//    // 第四个点位置信息
//    pVetAttr[3].pos[0] = 0.5f;
//    pVetAttr[3].pos[1] = 0.5f;
//    pVetAttr[3].pos[2] = 0.0f;
//    // 第四个点颜色信息
//    pVetAttr[3].color[0] = 0.0f;
//    pVetAttr[3].color[1] = 1.0f;
//    pVetAttr[3].color[2] = 1.0f;
//    // 第四个点的纹理坐标
//    pVetAttr[3].coord[0] = 1.0f;
//    pVetAttr[3].coord[1] = 1.0f;
//}

void CubeRenderWidget::setFillStatus(bool isFill)
{
    m_isFill = isFill;
    this->update();
}

bool CubeRenderWidget::isFill(void)
{
    return m_isFill;
}

void CubeRenderWidget::setMixNumber(float number)
{
    m_mixNumber = number;
    this->update();
}

float CubeRenderWidget::getMixNumber(void)
{
    return m_mixNumber;
}

// 设置旋转方向
void CubeRenderWidget::setRotationDirection(const QVector3D& vec)
{

}

QVector3D CubeRenderWidget::getRotationDirection(void)
{

}

// 添加纹理
void CubeRenderWidget::createTexture(const QImage& image, GLuint& textureid)
{
    glGenTextures(1, &textureid);
    glBindTexture(GL_TEXTURE_2D, textureid);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    // 设置纹理数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());

    glBindTexture(GL_TEXTURE_2D, 0);
}

void CubeRenderWidget::initModelData(void)
{
    m_points.clear();
    m_textureCoords.clear();
    m_normalVNs.clear();

    // 顶点数据
    m_points.push_back(QVector3D(1.000000f, 1.000000f, -1.000000f));
    m_points.push_back(QVector3D(1.000000f, -1.000000f, -1.000000f));
    m_points.push_back(QVector3D(1.000000f, 1.000000f, 1.000000f));
    m_points.push_back(QVector3D(1.000000f, -1.000000f, 1.000000f));
    m_points.push_back(QVector3D(-1.000000f, 1.000000f, -1.000000f));
    m_points.push_back(QVector3D(-1.000000f, -1.000000f, -1.000000f));
    m_points.push_back(QVector3D(-1.000000f, 1.000000f, 1.000000f));
    m_points.push_back(QVector3D(-1.000000f, -1.000000f, 1.000000f));

    // 纹理坐标
    m_textureCoords.push_back(QVector2D(0.625000f, 0.500000f));
    m_textureCoords.push_back(QVector2D(0.875000f, 0.500000f));
    m_textureCoords.push_back(QVector2D(0.875000f, 0.750000f));
    m_textureCoords.push_back(QVector2D(0.625000f, 0.750000f));
    m_textureCoords.push_back(QVector2D(0.375000f, 0.750000f));
    m_textureCoords.push_back(QVector2D(0.625000f, 1.000000f));
    m_textureCoords.push_back(QVector2D(0.375000f, 1.000000f));
    m_textureCoords.push_back(QVector2D(0.375000f, 0.000000f));
    m_textureCoords.push_back(QVector2D(0.625000f, 0.000000f));
    m_textureCoords.push_back(QVector2D(0.625000f, 0.250000f));
    m_textureCoords.push_back(QVector2D(0.375000f, 0.250000f));
    m_textureCoords.push_back(QVector2D(0.125000f, 0.500000f));
    m_textureCoords.push_back(QVector2D(0.375000f, 0.500000f));
    m_textureCoords.push_back(QVector2D(0.125000f, 0.750000f));

    // 法线
    m_normalVNs.push_back(QVector3D(0.0000f, 1.0000f, 0.0000f));
    m_normalVNs.push_back(QVector3D(0.0000f, 0.0000f, 1.0000f));
    m_normalVNs.push_back(QVector3D(-1.0000f, 0.0000f, 0.0000f));
    m_normalVNs.push_back(QVector3D(0.0000f, -1.0000f, 0.0000f));
    m_normalVNs.push_back(QVector3D(1.0000f, 0.0000f, 0.0000f));
    m_normalVNs.push_back(QVector3D(0.0000f, 0.0000f, -1.0000f));

    int facePoint[] = {1, 5, 7, 3, \
                       4, 3, 7, 8, \
                       8, 7, 5, 6, \
                       6, 2, 4, 8, \
                       2, 1, 3, 4, \
                       6, 5, 1, 2};

    int faceCoord[] = {1, 2, 3, 4, \
                       5, 4, 6, 7, \
                       8, 9, 10, 11, \
                       12, 13, 5, 14, \
                       13, 1, 4, 5, \
                       11, 10, 1, 13};

    int faceNoemal[] = {1, 1, 1, 1, \
                        2, 2, 2, 2, \
                        3, 3, 3, 3, \
                        4, 4, 4, 4, \
                        5, 5, 5, 5, \
                        6, 6, 6, 6};

    int count = sizeof(facePoint) / sizeof(int);
    for (int i=0; i<count; ++i)
    {
        VertexAttributeData vertexAttr;

        // 点坐标
        int index = facePoint[i] - 1;
        vertexAttr.pos[0] = m_points[index].x();
        vertexAttr.pos[1] = m_points[index].y();
        vertexAttr.pos[2] = m_points[index].z();

        // 纹理坐标
        index = faceCoord[i] - 1;
        vertexAttr.coord[0] = m_textureCoords[index].x();
        vertexAttr.coord[1] = m_textureCoords[index].y();

        // 法线
        index = faceNoemal[i] - 1;
        vertexAttr.normal[0] = m_normalVNs[index].x();
        vertexAttr.normal[1] = m_normalVNs[index].y();
        vertexAttr.normal[2] = m_normalVNs[index].z();

        m_cube.push_back(vertexAttr);
    }
}

void CubeRenderWidget::initTimer(void)
{
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(50);
    QObject::connect(m_pTimer, &QTimer::timeout, this, &CubeRenderWidget::onTimeout);
    m_pTimer->start();
}

void CubeRenderWidget::onTimeout(void)
{
    m_angle += 8;
    m_MMat.setToIdentity();
    m_MMat.translate(QVector3D(0, 0, -5));

    QVector3D vec(0.5f, 1.0f, 0.0f);
    m_MMat.rotate(m_angle, vec);

    this->update();
}
