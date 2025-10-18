#include "WeaChart/scenes/GLChartRenderer.h"

#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QOpenGLContext>

#include "WeaChart/scenes/GLChartview.h"
#include "WeaChart/utils/GLStructures.h"

#ifndef Q_OPENGL_DEBUG_LOGGER
void APIENTRY callBackOutput(GLenum source,
                             GLenum type,
                             GLuint id,
                             GLenum severity,
                             GLsizei length,
                             const GLchar *message,
                             const void *userParam)
{
    const QSet<int> fineErrors = {131169, 131185, 131218, 131204};
    // Ignoring fine errors
    if (fineErrors.contains(id)) return;

    qCritical() << "--------------------------";
    qCritical() << "OpenGL: " << __FILE__ << "at line: " << __LINE__;
    qCritical() << "Debug message (" << id << "): " << message;


    switch (source) {
    case GL_DEBUG_SOURCE_API: qCritical() << "Source API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: qCritical() << "Source Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: qCritical() << "Source Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: qCritical() << "Source Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION: qCritical() << "Source Application"; break;
    case GL_DEBUG_SOURCE_OTHER: qCritical() << "Source Other"; break;
    default: qCritical() << "Source Unknown Source"; break;
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR: qCritical() << "Type Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: qCritical() << "Type Deprecated Behavior"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: qCritical() << "Type Undefined Behavior"; break;
    case GL_DEBUG_TYPE_PORTABILITY: qCritical() << "Type Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: qCritical() << ""; break;
    case GL_DEBUG_TYPE_MARKER: qCritical() << "Type Performance"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP: qCritical() << "Type Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP: qCritical() << "Type Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER: qCritical() << "Type Other"; break;
    default: qCritical() << "Type Unknown Source"; break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: qCritical() << "Severity High"; break;
    case GL_DEBUG_SEVERITY_MEDIUM: qCritical() << "Severity Medium"; break;
    case GL_DEBUG_SEVERITY_LOW: qCritical() << "Severity Low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: qCritical() << "Severity Notification"; break;
    default: qCritical() << "Severity Unknown Source"; break;
    }
    qCritical() << "--------------------------";
}
#endif

GLChartRenderer::GLChartRenderer() :
    m_program(new QOpenGLShaderProgram()),
    m_programItems(new QOpenGLShaderProgram())
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

    glDeleteBuffers(1, &m_ssboSeriesProps);

    m_program->release();
    m_program->removeAllShaders();


    m_vaoItems.release();
    m_vaoItems.destroy();

    m_vboItems.release();
    m_vboItems.destroy();

    m_programItems->release();
    m_programItems->removeAllShaders();

    delete m_program;
    delete m_programItems;
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
    m_fboPointsSize = chartItem->m_pointsSize;
    m_fboSeriesProps = &chartItem->m_seriesProps;
    m_fboSeriesTexs = &chartItem->m_seriesTexs;
    auto propBg = chartItem->m_bg;
    if (propBg->hasNewData()) { // Avoid Everytime reading.
        m_fboBgColor = QColor(propBg->color());
        m_fboOpacity = (float)propBg->opacity();
        propBg->setDataRead(); // Clearing the Data Read
    }
    if (chartItem->m_hasNewItem || m_fboItems == nullptr) {
        m_fboItems = &chartItem->m_items;
        chartItem->m_hasNewItem = false;
    }
    if (chartItem->m_debug) initDebug();

    if (m_initBuffers) {
        updatePosVbo();
        if (m_fboItems && chartItem->m_hasItemsChanged) {
            allocateItemsVbo();
            chartItem->m_hasItemsChanged = false;
        }

    }

    if (!m_initialized) m_initialized = true; // Now Render Can be run after first synchronization.

}

void GLChartRenderer::render()
{
    if (!m_initialized) return;
    initBuffers();

    glViewport(0, 0, fboSize().first, fboSize().second);
    glClearColor(m_fboBgColor.redF(), m_fboBgColor.greenF(), m_fboBgColor.blueF(), m_fboOpacity);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawMainProgram();
    drawItemsProgram();
}

void GLChartRenderer::drawMainProgram()
{
    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.ortho(m_fboProj->left, m_fboProj->right, m_fboProj->bottom, m_fboProj->top, -1, 1);

    m_program->bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Uniforms
    m_program->setUniformValue("mvp", projection);
    m_program->setUniformValueArray("u_selectedRanges", m_fboSelectRange->data(), m_fboSelectRange->size());
    m_program->setUniformValue("u_texture", 0);

    int idx = 0;
    for (auto &prop : *m_fboSeriesProps) {
//        qDebug() << "Prop Start End: " << sizeof(projection) << prop.type << prop.markerShape << prop.startIndex << prop.endIndex << m_fboPoints->size();
        GLenum mode;
        switch (prop.type) {
        case GLSeriesType::SeriesTypeScatter:
            mode = GL_POINTS;
            break;
        case GLSeriesType::SeriesTypeLine:
            mode = GL_LINE_STRIP;
            break;
        case GLSeriesType::SeriesTypeArea:
            mode = GL_TRIANGLE_STRIP;
            break;
        default:
            qCritical() << "[ERROR]: at 'GLChartRenderer::render' class, Invalid handle.type!" << prop.type;
            break;
        }
//        qDebug() << "Props: " << idx << prop.markerShape << prop.markerSize << prop.type << prop.startIndex << prop.endIndex << m_fboSeriesProps->size() << m_fboPoints->size();
        if (prop.markerShape == GLMarkerShape::ShapeTexture &&
            prop.type == GLSeriesType::SeriesTypeScatter) {
            if (m_fboSeriesTexs->at(idx) == nullptr) {
                qCritical() << "[ERROR]: at 'GLChartRenderer::render' class, markerIcon wasn't set (is nullptr)!";
                exit(EXIT_FAILURE);
            }
            m_fboSeriesTexs->at(idx).data()->bind(0);
        }
        m_vao.bind();
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_ssboSeriesProps);
        glDrawArrays(mode, prop.startIndex, prop.endIndex - prop.startIndex);
        m_vao.release();
        if (prop.markerShape == GLMarkerShape::ShapeTexture &&
            prop.type == GLSeriesType::SeriesTypeScatter)
            m_fboSeriesTexs->at(idx).data()->release();

        idx++;
    }
    m_program->release();

}

void GLChartRenderer::drawItemsProgram()
{
    m_programItems->bind();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i(0); i < m_fboItems->size(); ++i) {
        const GLChartItemBase * item = m_fboItems->at(i);
        if (!item->isVisible()) continue;
        const QVector<QVector2D> &vertices = item->vertices();

        // Projection
        QMatrix4x4 mvp;
        mvp.setToIdentity();
        if (item->fixItem())
            mvp.ortho(0.0, 1.0, 0.0, 1.0, -1, 1);
        else if (item->isVertical())
            mvp.ortho(m_fboProj->left, m_fboProj->right, 0.0, 1.0, -1, 1);
        else
            mvp.ortho(0.0, 1.0, m_fboProj->bottom, m_fboProj->top, -1, 1);

        // Updating the uniforms
        m_programItems->setUniformValue("mvp", mvp);
        m_programItems->setUniformValue("u_z", (float) item->z());
        m_programItems->setUniformValue("u_texture", 0);
        m_programItems->setUniformValue("u_color", item->colorAsVec4D());
        m_programItems->setUniformValue("u_useImage", item->useImage());
        m_programItems->setUniformValue("u_useGradient", item->useImage());
        m_programItems->setUniformValue("u_isVertical", item->isVertical());
        m_programItems->setUniformValue("u_gradient", item->gradientAsVec4D());
        m_programItems->setUniformValue("u_lineStyle", (int) item->lineStyle());

        // Updating the VBO
        writeItemsVbo(vertices.size() * i * sizeof(QVector2D),
                      vertices.constData(),
                      vertices.size() * sizeof(QVector2D));

        // Binding Texture if need and available
        QOpenGLTexture *texture = nullptr;
        if (item->useImage() && !item->imageData().isNull()) {
            texture = new QOpenGLTexture(item->imageData());
            texture->bind(0);
        }

        // Drawing
        m_vaoItems.bind();
        glDrawArrays(item->drawType(), vertices.size() * i, vertices.size());
        m_vaoItems.release();

        // Releasing Texture
        if (item->useImage() && !item->imageData().isNull() && texture != nullptr) {
            texture->release(0);
            delete texture;
        }
    }
    m_programItems->release();
}

void GLChartRenderer::updatePosVbo()
{
    m_vboPoints.bind();
    m_vboPoints.allocate(m_fboPoints, sizeof(PointXYBase) * m_fboPointsSize);
    m_vboPoints.release();

    //    GLint size = -1;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssboSeriesProps);
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_fboSeriesProps->size() * sizeof(SeriesProps), m_fboSeriesProps->constData(), GL_DYNAMIC_DRAW);
    //    glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &size);
    //    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, m_fboSeriesProps->size() * sizeof(SeriesProps), m_fboSeriesProps->constData());
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void GLChartRenderer::allocateItemsVbo()
{
//    qDebug() << "on AlloCated: " << m_fboItems->size() << m_fboItems->size() * MAX_CHART_ITEM_VERTICES_COUNT;
    m_vboItems.bind();
    m_vboItems.allocate(sizeof(QVector2D) * m_fboItems->size() * MAX_CHART_ITEM_VERTICES_COUNT);
    m_vboItems.release();
}

void GLChartRenderer::writeItemsVbo(int offset, const QVector2D *data, int count)
{
    m_vboItems.bind();
    m_vboItems.write(offset, data, count);
    m_vboItems.release();
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

void GLChartRenderer::initDebug()
{
    if (m_debugActivate) return;
#ifdef Q_OPENGL_DEBUG_LOGGER
#else
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(callBackOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
    QOpenGLContext *ctx = QOpenGLContext::currentContext();
    ctx->hasExtension(QByteArrayLiteral("GL_KHR_debug"));

    QOpenGLDebugLogger *logger = new QOpenGLDebugLogger();
    logger->initialize();
    QObject::connect(logger, &QOpenGLDebugLogger::messageLogged,
                     logger, [=](const QOpenGLDebugMessage &debugMsg) { handleLoggedMessage(debugMsg); });
    logger->startLogging();

    m_debugActivate = true;
}

void GLChartRenderer::handleLoggedMessage(const QOpenGLDebugMessage &debugMsg)
{
    qCritical() << "--------------------------";
    qCritical() << "OpenGL: " << __FILE__ << "at line: " << __LINE__;
    qCritical() << "Debug message (" << debugMsg.id() << "): " << debugMsg.message();
    qCritical() << "--------------------------";
}

void GLChartRenderer::initShaders()
{
    linkProgram(m_program,
                ":/shadersChart/shaders/chart.vert", ":/shadersChart/shaders/chart.frag");
    linkProgram(m_programItems,
                ":/shadersChart/shaders/chartItem.vert", ":/shadersChart/shaders/chartItem.frag");
}

void GLChartRenderer::initBuffers()
{
    if (m_initBuffers) return;

    /** Main Program START **/
    m_vao.create();
    m_vboPoints.create();
    m_vboPoints.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    m_vao.bind();

    m_vboPoints.bind();
    m_vboPoints.allocate(sizeof(PointXYBase) *  m_fboPointsSize);

    glEnableVertexAttribArray(0); // position layout = 0
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(PointXYBase),
                          reinterpret_cast<void *>(offsetof(PointXYBase, position)));

    glEnableVertexAttribArray(1); // rgb layout = 1
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                          sizeof(PointXYBase),
                          reinterpret_cast<void *>(offsetof(PointXYBase, color)));

    m_vboPoints.release();

    glCreateBuffers(1, &m_ssboSeriesProps);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssboSeriesProps);
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_fboSeriesProps->size() * sizeof(SeriesProps), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_ssboSeriesProps);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    m_vao.release();
    /** Main Program END **/

    /** Items Program START **/
    m_vaoItems.create();
    m_vboItems.create();
    m_vboItems.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    m_vaoItems.bind();
    m_vboItems.bind();
    m_vboItems.allocate(m_fboItems->size() * sizeof(QVector2D) * MAX_CHART_ITEM_VERTICES_COUNT);

    glEnableVertexAttribArray(0); // position layout = 0
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QVector2D), nullptr);

    m_vboItems.release();
    m_vaoItems.release();

    m_initBuffers = true;

}

void GLChartRenderer::linkProgram(QOpenGLShaderProgram *prog,
                                  const QString &fileVertex, const QString &fileFragment)
{
    // Vertex + Fragment
    prog->create();
    bool vert_result = prog->addShaderFromSourceFile(QOpenGLShader::Vertex, fileVertex);
    if(!vert_result)
        qCritical() << "Cannot open " << fileVertex << vert_result;

    bool frag_result = prog->addShaderFromSourceFile(QOpenGLShader::Fragment,fileFragment);
    if(!frag_result)
        qCritical() << "Cannot open " << fileFragment << frag_result;

    if (!frag_result || !vert_result)
        exit(EXIT_FAILURE);

    if (!prog->link()) {
        qCritical() << "Failed to link Chart Shaders files!";
        exit(EXIT_FAILURE);
    }
}
