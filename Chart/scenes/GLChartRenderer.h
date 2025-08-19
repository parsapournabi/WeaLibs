#ifndef GLCHARTRENDERER_H
#define GLCHARTRENDERER_H

#include <QQuickFramebufferObject>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QPair>

struct Projection;
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
    bool m_initBuffers = false;
    bool m_initialized = false;

    // FramebufferObject Attributes (Scene)
    Projection *m_fboProj = nullptr;
    QVector<PointXYBase> m_fboPoints;
    QVector<QVector4D> *m_fboSelectRange = nullptr; // Selected Area
    QColor m_fboBgColor;
    float m_fboOpacity;
};

#endif // GLCHARTRENDERER_H
