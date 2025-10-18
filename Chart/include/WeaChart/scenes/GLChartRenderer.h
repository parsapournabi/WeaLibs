#ifndef GLCHARTRENDERER_H
#define GLCHARTRENDERER_H

#include <QQuickFramebufferObject>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>
#include <QPair>
#include "WeaChart/WeaChart_export.h"

#define Q_OPENGL_DEBUG_LOGGER

struct Projection;
struct SeriesProps;
struct PointXYBase;
class GLChartItemBase;

/// @brief Chart renderer based on QQuickFramebufferObject::Renderer
/// @details GLChartView FBO renderer, uses OpenGL 4.6 core. contains a glProgram.
/// single VertexArrayObject and also has single points VertexBufferObject and a ShaderStorageBuffer.
/// - MVP of renderer depends on `GLChartView axisRange(minX, maxX ...) and PAN/Zoom projection`.
/// - Three type of glDrawArrays it has. (GL_POINTS, GL_LINE_STRIP, GL_TRIANGLE_STRIP).
/// - Also texture sampler can attach on fragmentShader`.
/// - Blending is enable. updating buffers isn't like write subdata (every time it will write whole of the buffer).
class WEACHART_API GLChartRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions_4_5_Core
{
public:
    GLChartRenderer();
    ~GLChartRenderer();

    /// @brief The QQuickFramebufferObject::Renderer virtual method that must be override.
    /// @return FBO pointer.
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;
    /// @brief The QQuickFramebufferObject::Renderer virtual method that can be override.
    /// @details Synchronizing the Variables between Renderer & FBO.
    /// @param The FBO Object which should be cast into the GLChartView type.
    /// @note After the first synchronization call, The m_initialized will set to true.
    void synchronize(QQuickFramebufferObject *item) override;
    /// @brief The QQuickFramebufferObject::Renderer virtual method that must be override.
    /// @note The rendering process will not start until the m_initialized is set to true.
    void render() override;

    void drawMainProgram();
    void drawItemsProgram();

    /// @brief Writing the new dataset into the whole of the Vertex Buffer object.
    void updatePosVbo();

    /// @brief Allocating ChartItems Vertex Buffer.
    void allocateItemsVbo();
    /// @brief Writing each ChartItem properties into the ChartItem Vertex Buffer.
    void writeItemsVbo(int offset, const QVector2D *data, int count);

protected:
    /// @return-> first = width.
    /// @return-> second = height.
    const QPair<int, int> fboSize() const;

private:
    /// @brief Initializing OpenGL functions, debug mode and flags.
    void initGL();
    /// @brief Initializing OpenGL Debug Logger handler.
    void initDebug();
    /// @brief Debug Message handler
    void handleLoggedMessage(const QOpenGLDebugMessage &debugMsg);
    /// @brief Creating a OpenGL shader program and linking the Shader files with it.
    void initShaders();
    /// @brief The VertexArrayObject, Shader Storage buffer and The Vertex Buffer creation & allocation.
    /// @details After the method done the m_initBuffer will set to the true.
    void initBuffers();

    void linkProgram(QOpenGLShaderProgram *prog,
                     const QString &fileVertex, const QString &fileFragment);

    // Renderer Attributes
    /// @brief shader program.
    QOpenGLShaderProgram *m_program = nullptr, *m_programItems = nullptr;
    /// @brief Vertex Array Object (Stores VBO & SSBO and all other buffers).
    QOpenGLVertexArrayObject m_vao, m_vaoItems;
    /// @brief Vertex buffer Object Stores an array of point position + point color.
    QOpenGLBuffer m_vboPoints, m_vboItems;
    /// @brief Shader storage buffer object, stores all series information refers points.
    GLuint m_ssboSeriesProps;
    bool m_initBuffers = false;
    bool m_initialized = false;
    bool m_debugActivate = false;

    // FramebufferObject Attributes (Scene)
    Projection *m_fboProj = nullptr;
//    QVector<PointXYBase> *m_fboPoints = nullptr;
    PointXYBase *m_fboPoints = nullptr;
    size_t m_fboPointsSize;
    QVector<SeriesProps> *m_fboSeriesProps = nullptr;
    QVector<QSharedPointer<QOpenGLTexture>> *m_fboSeriesTexs = nullptr;
    QVector<QVector4D> *m_fboSelectRange = nullptr; // Selected Area
    QVector<GLChartItemBase *> *m_fboItems = nullptr;
    QColor m_fboBgColor;
    float m_fboOpacity;

};

#endif // GLCHARTRENDERER_H
