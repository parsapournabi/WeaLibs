#ifndef GLCHARTVIEW_H
#define GLCHARTVIEW_H

#include <QQuickFramebufferObject>
#include <QQmlEngine>
#include <QElapsedTimer>
#include <QBasicTimer>

#include "properties/PropertyBackground.h"
#include "properties/PropertyAxisRange.h"

#include "GLChartRenderer.h"
#include "GLStructures.h"
#include "GLSeriesHandle.h"

Q_DECLARE_METATYPE(Qt::MouseButton)
class GLChartView : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(PropertyBackground *background READ background CONSTANT)
    Q_PROPERTY(PropertyAxisRange *axisRange READ axisRange CONSTANT)
    Q_PROPERTY(qreal projLeft READ projLeft NOTIFY projUpdated)
    Q_PROPERTY(qreal projRight READ projRight NOTIFY projUpdated)
    Q_PROPERTY(qreal projBottom READ projBottom NOTIFY projUpdated)
    Q_PROPERTY(qreal projTop READ projTop NOTIFY projUpdated)
public:
    GLChartView(QQuickItem *parent = nullptr);
    GLChartRenderer *createRenderer() const override;
    const float normXtoWorld(const float &pX) const;
    const float normYtoWorld(const float &pY) const;

    // Properties
    PropertyBackground *background() const;
    PropertyAxisRange *axisRange() const;
    const qreal &projLeft() const;
    const qreal &projRight() const;
    const qreal &projBottom() const;
    const qreal &projTop() const;

    // Configuration & Process
    GLSeriesHandle *addSeries(QSharedPointer<IGLSeriesView> storage);
    const QVector<GLSeriesHandle*> &series() const;

    // STATIC
    static void registerMetaTypes() {
//        qRegisterMetaType<QVector<PointXYBase>>("QVector<PointXYBase>");

        qmlRegisterType<GLChartView>("GLItems", 1, 0, "GLChartView");
        qmlRegisterType<PropertyBackground>("GLItems", 1, 0, "GLBackgroundProperty");
        qmlRegisterType<PropertyAxisRange>("GLItems", 1, 0, "GLAxisRangeProperty");
    }
    static void addInstance(GLChartView *instance) { GLChartView::Instances.append(instance); }

    static GLChartView *getInstance(int index = 0) {
        if (GLChartView::Instances.isEmpty()) {
            GLChartView::addInstance(new GLChartView);
            return GLChartView::getInstance();
        }
        else if (index >= GLChartView::Instances.count()) {
            qCritical() << "Invalid index argument on GLChartView::getInstance! index: " << index
                        << " The return index is: " << GLChartView::Instances.count() - 1;
            return GLChartView::getInstance(GLChartView::Instances.count() - 1);
        }
        else
            return GLChartView::Instances.at(index);
    }

    inline static QVector<GLChartView *> Instances;

signals:
    /// @details Internal Signal.
    void projUpdated();

public slots:
    Q_INVOKABLE void updateSeries();

    Q_INVOKABLE void updateAxisRange();

    /// @details -> The slot will call with Mouse "onPressed" event.
    Q_INVOKABLE void preSelect(bool pClear);

    /// @details -> The slot will call with Mouse "onPositionChanged" event.
    Q_INVOKABLE void rangeSelecting(const QRect &pRectSelect);

    /// @details -> The slot will call with Mouse "onReleased" event.
    /// @details -> Method will emit Selected Points.
    Q_INVOKABLE void selectFinished() const;

protected:
    void makeConnections() const;

    void wheelEvent(QWheelEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void timerEvent(QTimerEvent *event) override;

private:
    void panAcceleration();

    // Test Methods
    void genPoints();

    // Base variables
    QBasicTimer m_timer;

    // PAN variables
    bool m_panning = false;
    QPointF m_panVelocity;
    QPointF m_lastMousePos;
    QElapsedTimer m_panElaps;
    int m_panTimerId = -1;

    // Renderer Interface Attributes
    Projection m_proj;
    QVector<PointXYBase> m_points;
//    ChartSeries m_nodes;
    QVector<QVector4D> m_selectRanges; // Selection Area
    qint8 m_selectRangeIdx = 0;

    QVector<GLSeriesHandle *> m_series;

    // Properties
    PropertyBackground *m_bg = nullptr;
    PropertyAxisRange *m_axisRange = nullptr;

    friend class GLChartRenderer;
};

#endif // GLCHARTVIEW_H
