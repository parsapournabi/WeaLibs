#ifndef GLCHARTRENDERER_H
#define GLCHARTRENDERER_H

#include <QQuickFramebufferObject>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QPair>

struct Projection;
struct SeriesProps;
struct PointXYBase;

class GLChartRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions_4_5_Core
{
public:
    GLChartRenderer();
    ~GLChartRenderer();

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;
    void synchronize(QQuickFramebufferObject *item) override;
    void render() override;

    void updatePosVbo();

protected:
    /// @return-> first = width.
    /// @return-> second = height.
    const QPair<int, int> fboSize() const;

private:
    void initGL();
    void initShaders();
    void initData();
    void initBuffers();


    // Renderer Attributes
    QOpenGLShaderProgram *m_program = nullptr;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vboPoints;
    GLuint m_ssboSeriesProps;
    bool m_initBuffers = false;
    bool m_initialized = false;

    // FramebufferObject Attributes (Scene)
    Projection *m_fboProj = nullptr;
//    QVector<PointXYBase> *m_fboPoints = nullptr;
    PointXYBase *m_fboPoints = nullptr;
    size_t m_fboPointsSize;
    QVector<SeriesProps> *m_fboSeriesProps = nullptr;
    QVector<QSharedPointer<QOpenGLTexture>> *m_fboSeriesTexs = nullptr;
    QVector<QVector4D> *m_fboSelectRange = nullptr; // Selected Area
    QColor m_fboBgColor;
    float m_fboOpacity;
};

#endif // GLCHARTRENDERER_H
