#ifndef GLCHARTVIEW_H
#define GLCHARTVIEW_H

#include <QQuickFramebufferObject>
#include <QQmlApplicationEngine>
#include <QElapsedTimer>

#include "WeaChart/properties/PropertyBackground.h"
#include "WeaChart/properties/PropertyAxisRange.h"

#include "GLChartRenderer.h"
#include "WeaChart/utils/GLStructures.h"
#include "WeaChart/series/GLSeriesHandle.h"
#include "WeaChart/series/GLSeriesItem.h"
#include "WeaChart/overlay/tools/GLChartVerticalLineItem.h"
#include "WeaChart/overlay/tools/GLChartHorizontalLineItem.h"
#include "WeaChart/overlay/tools/GLChartVerticalGateItem.h"
#include "WeaChart/overlay/tools/GLChartHorizontalGateItem.h"
#include "WeaChart/config.h"

/// @details Maximum number of points in all series.
/// @default default: 6'000'000
/// @note it allocate at GLChartView constructure -> MAX_CHART_TOTAL_POINTS * sizeof(PointXYBase).
constexpr int MAX_CHART_TOTAL_POINTS = 6'000'000;
/// @details Maximum VerticesCount in GLChartItemBase and its children classes.
constexpr int MAX_CHART_ITEM_VERTICES_COUNT = 4;

Q_DECLARE_METATYPE(Qt::MouseButton)

/// @brief An FBO class provides the UIs and the logics of the Chart scene.
/// @details Chart scene FBO, provides chart projection, process, series handling, legeds properties assignment.
/// Chart scene PAN/Zoom and points selection.
/// @note The object must be created in QML, and in C++ it can be accesssed using the `getInstance` method,
/// which accepts an index argument. (The index value depends on maximum number of Charts created.
class WEACHART_API GLChartView : public QQuickFramebufferObject
{
    Q_OBJECT
    /// @brief The FBO Background style such as color and opacity.
    Q_PROPERTY(PropertyBackground *background READ background CONSTANT)
    /// @brief The default top, left, bottom right value of the FBO's Projection.
    Q_PROPERTY(PropertyAxisRange *axisRange READ axisRange CONSTANT)
    /// @brief Limiting the field of view refers to the axisRange.
    Q_PROPERTY(bool limitView READ limitView WRITE setLimitView NOTIFY limitViewChanged)
    /// @brief Projection left value.
    /// @details Property used on QML GLChartFrame.
    Q_PROPERTY(qreal projLeft READ projLeft WRITE setProjLeft NOTIFY updateQml)
    /// @brief Projection right value.
    /// @details Property used on QML GLChartFrame.
    Q_PROPERTY(qreal projRight READ projRight WRITE setProjRight NOTIFY updateQml)
    /// @brief Projection bottom value.
    /// @details Property used on QML GLChartFrame.
    Q_PROPERTY(qreal projBottom READ projBottom WRITE setProjBottom NOTIFY updateQml)
    /// @brief Projection top value.
    /// @details Property used on QML GLChartFrame.
    Q_PROPERTY(qreal projTop READ projTop WRITE setProjTop NOTIFY updateQml)
    /// @brief provides GLAbstractSeries properties as a QVariantList.
    /// @details Property used on QML GLItemLegend
    Q_PROPERTY(QVariantList legendItems READ legendItems NOTIFY updateLegends)
    /// @brief Adjusting the PAN smoothness factor for movement.
    /// @details The value range is between 0.0 and 1.0
    /// @default default: 0.91
    Q_PROPERTY(qreal velocityCoefficient MEMBER velocityCoefficient)
    /// @brief The PAN button (it should be leftButton or rightButton or middleButton)
    /// @default default: Qt::MiddleButton.
    Q_PROPERTY(Qt::MouseButton panMouseButton MEMBER panMouseButton)
    Q_ENUM(Qt::MouseButton)
    /// @brief Auto Scaling Policy with Minimum and maximum vertex in the chart.
    /// @note Enabling Auto Scale & fitWindow will decrease performance.
    Q_PROPERTY(GLAutoScalePolicy autoScalePolicy READ autoScalePolicy WRITE setAutoScalePolicy NOTIFY autoScalePolicyChanged)
    /// @brief A boolean to activate windows fit (autoScale) or not.
    /// @note Enabling Auto Scale & fitWindow will decrease performance.
    Q_PROPERTY(bool fitWindow READ fitWindow WRITE setFitWindow NOTIFY fitWindowChanged)

    /// @brief A boolean to activate OpenGL GLChartRenderer debugmode
    Q_PROPERTY(bool debug READ debug WRITE setDebug CONSTANT)

public:
    GLChartView(QQuickItem *parent = nullptr);
    ~GLChartView();
    /// @brief required virtual method on the QQuickFramebufferObject that should be overridden.
    GLChartRenderer *createRenderer() const override;

    /// @brief Automatically Reading GLSeriesItem {} which are implemented as child in GLChartFrame (QML).
    /// GLChartFrame {
    /// 	GLSeriesItem {
    /// 		id: series0
    ///
    /// 	}
    /// 	GLSeriesItem {
    /// 		id: series1
    /// 	}
    /// }
    /// In this situation, process will find 2 children of type `GLSeriesItem`, and then pass each of them to the
    /// addSeriesPtr method.
    void readQmlComponents();

    // Properties
    /// @brief getter of the background property.
    PropertyBackground *background() const;
    /// @brief getter of the axisRange property.
    PropertyAxisRange *axisRange() const;
    /// @brief getter of the limitView property.
    bool limitView() const;
    /// @brief setter of the limitView property.
    void setLimitView(bool doLimit);
    /// @brief getter of the projLeft property.
    const qreal &projLeft() const;
    /// @brief setter of the projLeft property.
    void setProjLeft(const qreal &value);
    /// @brief getter of the projRight property.
    const qreal &projRight() const;
    /// @brief setter of the projRight property.
    void setProjRight(const qreal &value);
    /// @brief getter of the projBottom property.
    const qreal &projBottom() const;
    /// @brief setter of the projBottom property.
    void setProjBottom(const qreal &value);
    /// @brief getter of the projTop property.
    const qreal &projTop() const;
    /// @brief setter of the projTop property.
    void setProjTop(const qreal &value);
    /// @brief getter of the legendItems property.
    /// @details legendItems provides GLLegend properties, which are used in GLItemLegend Repeater model.
    QVariantList legendItems() const;

    /// @brief getter of the autoScalePolicy property.
    /// @return GLAutoScalePolicy.
    GLAutoScalePolicy autoScalePolicy() const noexcept;
    /// @brief setter of the autoScalePolicy property.
    /// @param GLAutoScalePolicy.
    void setAutoScalePolicy(GLAutoScalePolicy policy) noexcept;

    /// @brief getter of the fitWindow property.
    /// @return bool.
    bool fitWindow() const noexcept;
    /// @brief setter of the fitWindow property.
    /// @param bool.
    void setFitWindow(bool fit) noexcept;

    /// @brief getter of the debug property.
    /// @return bool.
    bool debug() const;
    /// @brief setter of the debug property.
    /// @param bool.
    void setDebug(bool active);

    // Configuration & Process
    /// @brief Adding a GLSeriesHandle using storage param, then the handler will add into the m_view member.
    /// @param storage -> series with type of GLSeriesStorage<T>.
    /// @return Created handler with type of the GLSeriesHandle
    GLSeriesHandle *addSeries(GLAbstractSeries *storage);
    GLSeriesHandle *addSeriesPtr(GLAbstractSeries *series);
    /// @return All GLSeriesHandle (also GLAbstractSeries) which contains in GLChartFrame
    const QVector<GLSeriesHandle*> &handles() const;

    /// @brief Adding GLChartItemBase base class to FBO.
    bool addItem(GLChartItemBase *item);
    /// @brief removing GLChartItemBase from FBO.
    bool removeItem(GLChartItemBase *item);
    /// @return All GLChartItemBase which contains in GLChartFrame
    const QVector<GLChartItemBase *> &chartItems() const;


    // STATIC
    /// @details Registering all dataTypes for C++ and QML.
    /// @note Import path on qml -> com.wearily.WeaChart 1.0
    /// @throw assertion if the WeaChart_QML_IMPORT_PATH isn't define or it has empty value.
    static void registerMetaTypes(QQmlApplicationEngine *engine) {
        qRegisterMetaType<GLSeriesType>("GLSeriesType");
        qRegisterMetaType<GLColorType>("GLColorType");
        qRegisterMetaType<GLMarkerShape>("GLMarkerShape");

        qmlRegisterUncreatableType<GLMetaEnums>("com.wearily.WeaChart", 1, 0, "GL", "Only Enums class");
        qmlRegisterType<PropertySeries>("com.wearily.WeaChart", 1, 0, "GLSeries");
        qmlRegisterType<GLSeriesItem>("com.wearily.WeaChart", 1, 0, "GLSeriesItem");
        qmlRegisterType<GLChartView>("com.wearily.WeaChart", 1, 0, "GLChartView");
        qmlRegisterType<PropertyBackground>("com.wearily.WeaChart", 1, 0, "GLBackgroundProperty");
        qmlRegisterType<PropertyAxisRange>("com.wearily.WeaChart", 1, 0, "GLAxisRangeProperty");

        // ChartItems
        qmlRegisterType<GLChartVerticalLineItem>("com.wearily.WeaChart", 1, 0, "GLChartVerticalLineItem");
        qmlRegisterType<GLChartHorizontalLineItem>("com.wearily.WeaChart", 1, 0, "GLChartHorizontalLineItem");
        qmlRegisterType<GLChartVerticalGateItem>("com.wearily.WeaChart", 1, 0, "GLChartVerticalGateItem");
        qmlRegisterType<GLChartHorizontalGateItem>("com.wearily.WeaChart", 1, 0, "GLChartHorizontalGateItem");
#ifndef WeaChart_QML_IMPORT_PATH
        static_assert(false,
                      "[ERROR]: Undefined WeaChart_QML_IMPORT_PATH. undefined WeaChart *.qml files.");
#else
        static_assert(sizeof(WeaChart_QML_IMPORT_PATH) > 1,
                      "[ERROR]: WeaChart_QML_IMPORT_PATH cannot be empty. undefined WeaChart *.qml files.");

        // REQUIRED to recognize .qml files.
        engine->addImportPath(WeaChart_QML_IMPORT_PATH);

#endif
    }
    /// @brief adding the GLChartView object into the GLChartView::Instances vector.
    /// @param the object with type of GLChartView.
    static void addInstance(GLChartView *instance) { GLChartView::Instances.append(instance); }

    /// @brief Getting the specific object from the GLChartView::Instances.
    /// @param Object index.
    /// @return The specific object as pointer.
    /// @note If the index param is not in range of the GLChartView:::Instances, The object will create automatically.
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

    void autoScalePolicyChanged();

    void fitWindowChanged();

public slots:
    /// @brief Converting X pixels to the World x position(refers to the projection).
    /// @param pX -> The value of the pixel.
    /// @return The converted X value.
    Q_INVOKABLE float normXtoWorld(const float &pX) const;
    /// @brief Converting Y pixels to the World y position(refers to the projection).
    /// @param pY -> The value of the pixel.
    /// @return The converted Y value.
    Q_INVOKABLE float normYtoWorld(const float &pY) const;

    /// @details updating all series data.
    Q_INVOKABLE void updateSeries();

    /// @brief Reset the projection to the default axis values.
    Q_INVOKABLE void updateAxisRange();

    /// @brief Crop the chart scene using the rubberBand
    /// @param pRectView is the Rectangle position & size, unit by pixels.
    Q_INVOKABLE void updateRectView(const QRect &pRectView);

    /// @brief on mouse button triggered.
    /// @details -> The slot will call with Mouse "onPressed" event.
    Q_INVOKABLE void preSelect(bool pClear);

    /// @brief During the selection (mouseMove & mousePressed).
    /// @details -> The slot will call with Mouse "onPositionChanged" event.
    /// @param pRectSelect is the Rectangle position & size, unit by pixles.
    Q_INVOKABLE void rangeSelecting(const QRect &pRectSelect);

    /// @brief -> The slot will call with Mouse "onReleased" event.
    /// @details -> Method will emit Selected Points.
    Q_INVOKABLE void selectFinished() const;

    /// @brief Zoom event slot.
    /// @details ->  Zoom In/Out using Mouse x, y, angleDelta convertion to WorldView.
    /// @param -> mxPixel: float => mouse x position.
    /// @param -> myPixel: float => mouse y position.
    /// @param -> angleDelta: float => mouse angleDeltaY
    /// @param -> zoomX: bool = @default = true => if false the X axis Zoom In/Out will ignore.
    /// @param -> zoomY: bool = @default = true => if false the Y axis Zoom In/Out will ignore.
    Q_INVOKABLE void adjustView(float mxPixel, float myPixel, float angleDeltaY,
                                bool zoomX = true, bool zoomY = true);

protected:
    /// @brief Signal/Slot connections during initialization.
    void makeConnections() const;

    /// @details Zoom In/Out function.
    void wheelEvent(QWheelEvent *event) override;

    /// @details PAN Start process.
    void mousePressEvent(QMouseEvent *event) override;

    /// @details Reset field of view to axisRange default values. (minX, maxX and ...).
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /// @details PANNING process
    void mouseMoveEvent(QMouseEvent *event) override;

    /// @details User pan finished but the deceleration ramp starts.
    void mouseReleaseEvent(QMouseEvent *event) override;

    /// @details PAN velocity timer.
    void timerEvent(QTimerEvent *event) override;

private:
    /// @details Method will call when user released mouse button.
    /// Ramp velocity decrease based on velocityCoefficient property factor.
    /// Projection moves until m_panVelocity is less than 0.03.
    void panAcceleration();

    void resetProjection();
    void autoScaleUpdateAxis();
    void autoScaleUpdateAxis(const GLAbstractSeries &serie);

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
    QVector<GLChartItemBase *> m_items;
    bool m_hasNewItem = false;
    bool m_hasItemsChanged = false;

    // Properties
    PropertyBackground *m_bg = nullptr; // Referer QML
    PropertyAxisRange *m_axisRange = nullptr; // Referer QML
    bool m_limitView = false; // Referer QML
    qreal velocityCoefficient = 0.91; // Referer QML
    Qt::MouseButton panMouseButton = Qt::MiddleButton; // Referer QML
    GLAutoScalePolicy m_autoScalePolicy = GLAutoScalePolicy::PolicyNone;
    bool m_pvtAutoScale = true; // Internal Enabling autoScale
    bool m_fitWindow = false;
    bool m_debug = false;

    friend class GLChartRenderer;
};

#endif // GLCHARTVIEW_H
