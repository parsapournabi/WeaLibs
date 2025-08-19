#include "GLChartRenderer.h"

#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFunctions>

#include "GLChartview.h"
#include "GLStructures.h"

GLChartRenderer::GLChartRenderer() :
    m_program(new QOpenGLShaderProgram())
{
    initGL();
    initShaders();

//    m_initialized = true;
}

GLChartRenderer::~GLChartRenderer()
{
    m_vao.release();
    m_vao.destroy();

    m_vboPoints.release();
    m_vboPoints.destroy();

    m_program->release();
    m_program->removeAllShaders();

    delete m_program;
}

QOpenGLFramebufferObject *GLChartRenderer::createFramebufferObject(const QSize &size)
{
//    qDebug() << "createFramebufferObject called!";
    QOpenGLFramebufferObjectFormat fmt;
    fmt.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    fmt.setSamples(4);
    return new QOpenGLFramebufferObject(size, fmt);

}

void GLChartRenderer::synchronize(QQuickFramebufferObject *item)
{
    GLChartView *chartItem = qobject_cast<GLChartView *>(item);

    m_fboProj = &chartItem->m_proj;
    m_fboSelectRange = &chartItem->m_selectRanges;
    m_fboPoints = chartItem->m_points;
    auto propBg = chartItem->m_bg;
    if (propBg->hasNewData()) { // Avoid Everytime reading.
        m_fboBgColor = QColor(propBg->color());
        m_fboOpacity = (float)propBg->opacity();
        propBg->setDataRead(); // Clearing the Data Read
    }

    if (m_initBuffers) {
        updatePosVbo();
//        updateRgbVbo();
//        updateAlphaVbo();
    }

    if (!m_initialized) m_initialized = true; // Now Render Can be run after first synchronization.

}

void GLChartRenderer::render()
{
    if (!m_initialized) return;
    initData();
    initBuffers();

    glViewport(0, 0, fboSize().first, fboSize().second);
    glClearColor(m_fboBgColor.redF(), m_fboBgColor.greenF(), m_fboBgColor.blueF(), m_fboOpacity);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.ortho(m_fboProj->left, m_fboProj->right, m_fboProj->bottom, m_fboProj->top, -1, 1);

    m_program->bind();

    // Uniforms
    m_program->setUniformValue("mvp", projection);
    m_program->setUniformValueArray("u_selectedRanges", m_fboSelectRange->data(), m_fboSelectRange->size());

    m_vao.bind();
    glDrawArrays(GL_POINTS, 0, m_fboPoints.size());
    m_vao.release();

    m_program->release();
}

void GLChartRenderer::updatePosVbo()
{
    m_vboPoints.bind();
    m_vboPoints.allocate(m_fboPoints.constData(), m_fboPoints.size() * sizeof(PointXYBase));
    m_vboPoints.release();
}

const QPair<int, int> GLChartRenderer::fboSize() const
{
    return QPair<int, int>(framebufferObject()->width(), framebufferObject()->height());
}

void GLChartRenderer::initGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_PROGRAM_POINT_SIZE);

}

void GLChartRenderer::initShaders()
{
    // Vertex + Fragment
    m_program->create();
    bool vert_result = m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                          ":/shadersChart/chart.vert");
    if(!vert_result)
        qCritical() << "Cannot open chart.vert!" << vert_result;
    bool frag_result = m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                          ":/shadersChart/chart.frag");
    if(!frag_result)
        qCritical() << "Cannot open chart.frag!" << frag_result;
    if (!frag_result || !vert_result)
        exit(EXIT_FAILURE);

    if (!m_program->link()) {
        qCritical() << "Failed to link Chart Shaders files!";
        exit(EXIT_FAILURE);
    }
}

void GLChartRenderer::initData()
{
    if (m_initBuffers) return;

}

void GLChartRenderer::initBuffers()
{
    if (m_initBuffers) return;

    m_vao.create();
    m_vboPoints.create();

    m_vao.bind();

    m_vboPoints.bind();
    m_vboPoints.allocate(m_fboPoints.size() * sizeof(PointXYBase));

    glEnableVertexAttribArray(0); // position layout = 0
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(PointXYBase),
                          reinterpret_cast<void *>(offsetof(PointXYBase, position)));

    glEnableVertexAttribArray(1); // rgb layout = 1
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                          sizeof(PointXYBase),
                          reinterpret_cast<void *>(offsetof(PointXYBase, color)));
    m_vboPoints.release();

    m_vao.release();

    m_initBuffers = true;

}
