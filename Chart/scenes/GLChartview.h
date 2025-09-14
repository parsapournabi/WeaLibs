#ifndef GLCHARTVIEW_H
#define GLCHARTVIEW_H

#include <QQuickFramebufferObject>
#include <QQmlEngine>
#include <QElapsedTimer>
#include <QBasicTimer>
#include <QQmlListProperty>
#include <QMutex>

#include "properties/PropertyBackground.h"
#include "properties/PropertyAxisRange.h"

#include "GLChartRenderer.h"
#include "GLStructures.h"
#include "GLSeriesHandle.h"
#include "GLSeriesItem.h"

constexpr int MAX_CHART_TOTAL_POINTS = 6'000'000;

Q_DECLARE_METATYPE(Qt::MouseButton)
class GLChartView : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(PropertyBackground *background READ background CONSTANT)
    Q_PROPERTY(PropertyAxisRange *axisRange READ axisRange CONSTANT)
    Q_PROPERTY(bool limitView READ limitView WRITE setLimitView NOTIFY limitViewChanged)
    Q_PROPERTY(qreal projLeft READ projLeft NOTIFY updateQml)
    Q_PROPERTY(qreal projRight READ projRight NOTIFY updateQml)
    Q_PROPERTY(qreal projBottom READ projBottom NOTIFY updateQml)
    Q_PROPERTY(qreal projTop READ projTop NOTIFY updateQml)
    Q_PROPERTY(QVariantList legendItems READ legendItems NOTIFY updateLegends)
    Q_PROPERTY(qreal velocityCoefficient MEMBER velocityCoefficient) // Between 0~1.0
    Q_PROPERTY(Qt::MouseButton panMouseButton MEMBER panMouseButton)
    Q_ENUM(Qt::MouseButton)
public:
    GLChartView(QQuickItem *parent = nullptr);
    ~GLChartView();
    GLChartRenderer *createRenderer() const override;
    void getSeriesFromQml();
    const float normXtoWorld(const float &pX) const;
    const float normYtoWorld(const float &pY) const;

    // Properties
    PropertyBackground *background() const;
    PropertyAxisRange *axisRange() const;
    bool limitView() const;
    void setLimitView(bool doLimit);
    const qreal &projLeft() const;
    const qreal &projRight() const;
    const qreal &projBottom() const;
    const qreal &projTop() const;
    QVariantList legendItems() const;

    // Configuration & Process
    GLSeriesHandle *addSeries(QSharedPointer<GLAbstractSeries> storage);
    GLSeriesHandle *addSeriesPtr(GLAbstractSeries *series);
    Q_INVOKABLE const QVector<GLSeriesHandle*> &handles() const;

    // STATIC
    static void registerMetaTypes() {
//        qRegisterMetaType<PointXYBase>("PointXYBase");
//        qRegisterMetaType<QVector<PointXYBase>>("QVector<PointXYBase>");

        qRegisterMetaType<GLSeriesType>("GLSeriesType");
        qRegisterMetaType<GLColorType>("GLColorType");
        qRegisterMetaType<GLMarkerShape>("GLMarkerShape");

        qmlRegisterUncreatableType<GLMetaEnums>("GLItems", 1, 0, "GL", "Only Enums class");
        qmlRegisterType<PropertySeries>("GLItems", 1, 0, "GLSeries");
        qmlRegisterType<GLSeriesItem>("GLItems", 1, 0, "GLSeriesItem");
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
    void updateQml();

    /// @details Signal will use on Legend ListModel updating.
    void updateLegends();

    void limitViewChanged();

public slots:

    Q_INVOKABLE void updateSeries();

    Q_INVOKABLE void updateAxisRange();

    Q_INVOKABLE void updateRectView(const QRect &pRectView);

    /// @details -> The slot will call with Mouse "onPressed" event.
    Q_INVOKABLE void preSelect(bool pClear);

    /// @details -> The slot will call with Mouse "onPositionChanged" event.
    Q_INVOKABLE void rangeSelecting(const QRect &pRectSelect);

    /// @details -> The slot will call with Mouse "onReleased" event.
    /// @details -> Method will emit Selected Points.
    Q_INVOKABLE void selectFinished() const;

    /// @details ->  Zoom In/Out using Mouse x, y, angleDelta convertion to WorldView.
    /// @param -> mxPixel: float => mouse x position.
    /// @param -> myPixel: float => mouse y position.
    /// @param -> angleDelta: float => mouse angleDeltaY
    /// @param -> zoomX: bool = @default = true => @if false the X axis Zoom In/Out will ignore.
    /// @param -> zoomY: bool = @default = true => @if false the Y axis Zoom In/Out will ignore.
    Q_INVOKABLE void adjustView(float mxPixel, float myPixel, float angleDeltaY,
                                bool zoomX = true, bool zoomY = true);

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

    // Base variables
    mutable bool m_initSeries = false;

    // PAN variables
    bool m_panning = false;
    QPointF m_panVelocity;
    QPointF m_lastMousePos;
    QElapsedTimer m_panElaps;
    int m_panTimerId = -1;

    // Renderer Interface Attributes
    Projection m_proj;
//    QVector<PointXYBase> m_points;
    PointXYBase *m_points = nullptr;
    size_t m_pointsSize;
    QVector<SeriesProps> m_seriesProps; // Properies from GLAbstractSeries & children classes. (Q_PROPERTY).
    QVector<QSharedPointer<QOpenGLTexture>> m_seriesTexs;
    QVector<QVector4D> m_selectRanges; // Selection Area
    qint8 m_selectRangeIdx = 0;

    QVector<GLSeriesHandle *> m_series;

    // Properties
    PropertyBackground *m_bg = nullptr; // Referer QML
    PropertyAxisRange *m_axisRange = nullptr; // Referer QML
    bool m_limitView = false; // Referer QML
    qreal velocityCoefficient = 0.91; // Referer QML
    Qt::MouseButton panMouseButton = Qt::MiddleButton; // Referer QML

    friend class GLChartRenderer;
};

#endif // GLCHARTVIEW_H
