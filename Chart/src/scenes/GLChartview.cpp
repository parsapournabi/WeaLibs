#include "WeaChart/scenes/GLChartview.h"

#include <QRandomGenerator>
#include <QThread>
#include <QTimer>

GLChartView::GLChartView(QQuickItem *parent) :
    QQuickFramebufferObject{parent},
    m_bg(new PropertyBackground(this)),
    m_axisRange(new PropertyAxisRange(this)),
    m_points(new PointXYBase[MAX_CHART_TOTAL_POINTS])
{
    GLChartView::addInstance(this);

    // Initialization
    m_selectRanges.resize(32); // Referer chart.vert
    memset(m_selectRanges.data(), 0, m_selectRanges.size() * sizeof(QVector4D));
    m_seriesTexs.reserve(32);

    // options
    setMirrorVertically(true);
    setAcceptedMouseButtons(Qt::AllButtons);

    makeConnections();
}

GLChartView::~GLChartView()
{
    delete [] m_points;
}

GLChartRenderer *GLChartView::createRenderer() const
{
    if (!m_initSeries) {
        auto self = const_cast<GLChartView *>(this);
        QMetaObject::invokeMethod(self, &GLChartView::readQmlComponents);
        m_initSeries = true;
    }
    return new GLChartRenderer();
}

void GLChartView::readQmlComponents()
{
    QList<QQuickItem *> childItems = parentItem()->parentItem()->childItems();
    for (QQuickItem *child : childItems) {
        // Reading QML GLSeriesItem.
        GLSeriesItem *series = qobject_cast<GLSeriesItem *>(child);
        if (series) addSeriesPtr(series->series());
        // Reading QML GLChartItemBase
        GLChartItemBase *item = qobject_cast<GLChartItemBase *>(child);
        if (item) addItem(item);
    }

}

float GLChartView::normXtoWorld(const float &pX) const
{
    return ((pX - 0) / (width() - 0)) * (m_proj.right - m_proj.left) + m_proj.left;
}

float GLChartView::normYtoWorld(const float &pY) const
{
    return ((height() - (pY - 0)) / (height() - 0)) * (m_proj.top - m_proj.bottom) + m_proj.bottom;
}

PropertyBackground *GLChartView::background() const
{
    return m_bg;
}

PropertyAxisRange *GLChartView::axisRange() const
{
    return m_axisRange;
}

bool GLChartView::limitView() const
{
    return m_limitView;
}

void GLChartView::setLimitView(bool doLimit)
{
    m_limitView = doLimit;
    emit limitViewChanged();
}

const qreal &GLChartView::projLeft() const
{
    return m_proj.left;
}

void GLChartView::setProjLeft(const qreal &value)
{
    if (value == m_proj.left) return;

    m_proj.left = value;
    emit updateQml();
    update();
}

const qreal &GLChartView::projRight() const
{
    return m_proj.right;
}

void GLChartView::setProjRight(const qreal &value)
{
    if (value == m_proj.right) return;

    m_proj.right = value;
    emit updateQml();
    update();

}

const qreal &GLChartView::projBottom() const
{
    return m_proj.bottom;
}

void GLChartView::setProjBottom(const qreal &value)
{
    if (value == m_proj.bottom) return;

    m_proj.bottom = value;
    emit updateQml();
    update();
}

const qreal &GLChartView::projTop() const
{
    return m_proj.top;
}

void GLChartView::setProjTop(const qreal &value)
{
    if (value == m_proj.top) return;

    m_proj.top = value;
    emit updateQml();
    update();
}

QVariantList GLChartView::legendItems() const
{
    QVariantList legends;
    for (auto &handle : m_series) {
        const auto &view = handle->view();
        const QMetaObject *metaObj = view.metaObject();
        QMap<QString, QVariant>  legend;
        for (int i = 0; i < metaObj->propertyCount(); ++i) {
            QMetaProperty prop = metaObj->property(i);
            if (!prop.isReadable()) qDebug() << "Prop isn't Readable: " << prop.name();
            legend[prop.name()] = prop.read(&view);
        }
        legends.append(legend);
    }
    //    qDebug() << "LEGENDS ITEM: " << legends;
    return legends;
}

GLAutoScalePolicy GLChartView::autoScalePolicy() const noexcept
{
    return m_autoScalePolicy;
}

void GLChartView::setAutoScalePolicy(GLAutoScalePolicy policy) noexcept
{
    if (policy == m_autoScalePolicy) return;
    m_autoScalePolicy = policy;
    emit autoScalePolicyChanged();
    updateAxisRange();
}

bool GLChartView::fitWindow() const noexcept
{
    return m_fitWindow;
}

void GLChartView::setFitWindow(bool fit) noexcept
{
    if (fit == m_fitWindow) return;
    m_fitWindow = fit;
    emit fitWindowChanged();
    updateAxisRange();
}

bool GLChartView::debug() const
{
    return m_debug;
}

void GLChartView::setDebug(bool active)
{
    m_debug = active;
}

GLSeriesHandle *GLChartView::addSeries(GLAbstractSeries *storage)
{
    auto handle = new GLSeriesHandle(storage, this);
    m_series.append(handle);

    //    connect(&handle->view(), &GLAbstractSeries::changed, this, &GLChartView::updateQml);
    connect(&handle->view(), &GLAbstractSeries::changed, this, &GLChartView::updateLegends);
    connect(&handle->view(), &GLAbstractSeries::changed, this, &GLChartView::updateSeries);
    connect(&handle->view(), &GLAbstractSeries::signalUpdateSeriesData, this, &GLChartView::updateSeries);
//    connect(&handle->view(), &GLAbstractSeries::changed, this, [=] () {qDebug() << "GLAbstractSeries Changed emitted!";});

    emit updateLegends();
    emit updateQml();

    updateSeries();

    return handle;
}

GLSeriesHandle *GLChartView::addSeriesPtr(GLAbstractSeries *series)
{
    if (series == nullptr) {
        qCritical() << "[ERROR]: at 'GLChartView::addSeriesPtr' class, 'series' is nullptr!"
                    << "Make sure you the series property at 'GLSeriesItem' (QML) isn't nullptr.";
        exit(EXIT_FAILURE);
    }
//    QSharedPointer<GLAbstractSeries> shared(series);
    return addSeries(series);
}

const QVector<GLSeriesHandle *> &GLChartView::handles() const
{
    return m_series;
}

bool GLChartView::addItem(GLChartItemBase *item)
{
    if (item == nullptr || m_items.contains(item)) return false;
    m_items.append(item);
    connect(item, &GLChartItemBase::changed, this, [=]() {
        m_hasItemsChanged = true;
        update();
    });

    m_hasItemsChanged = true;
    m_hasNewItem = true;
    update();

    return true;
}

bool GLChartView::removeItem(GLChartItemBase *item)
{
    if (item == nullptr) return false;
    for (int i(0); i < m_items.size(); ++i) {
        if (m_items[i] != item) continue;
        delete m_items[i];
        m_items.removeAt(i);
        m_hasNewItem = true;
        return true;
    }
    return false;
}

const QVector<GLChartItemBase *> &GLChartView::chartItems() const
{
    return m_items;
}

void GLChartView::updateSeries()
{
    QVector<SeriesProps> seriesProps;
    m_seriesTexs.clear();
    int offset = 0;
    for (auto &handle : m_series) {
        const auto &view = handle->view();
        if (!view.visible())
            continue;
        if (offset + view.count() > MAX_CHART_TOTAL_POINTS)
            qCritical() << "[ERROR]: at 'GLChartView' class, total series points is bigger that 'MAX_CHART_TOTAL_POINTS'!"
                        << "TotalPoints: " << offset + view.count() << "MAX_CHART_TOTAL_POINTS: " << MAX_CHART_TOTAL_POINTS;

        // Auto Scale Projection update if enable
        autoScaleUpdateAxis(view);

        view.basePoints(m_points, offset);
        //        PointXYBase *points = view.basePoints();
        //        for (int i = offset; i < offset + view.count(); ++i) {
        //            qDebug() << i << "Point At Series: " << view.count() << m_points[i].position;
        //        }

        QSharedPointer<QOpenGLTexture> texPtr = nullptr;
        if (view.markerIconUrl().length())
            texPtr = QSharedPointer<QOpenGLTexture>::create(*view.markerIconPtr());
//        qDebug() << "TEXTURE: " << texPtr;
        if (view.type() == GLSeriesType::SeriesTypeScatter) { // Because in glDrawArrays must be startswith(GL_POINTS).
            seriesProps.insert(0,
                               {
                                   offset,
                                   view.count() + offset,
                                   view.markerShape(),
                                   view.markerSize(),
                                   view.colorType(),
                                   view.type(),
                                   0,
                                   QVector4D(view.color().redF(),
                                             view.color().greenF(),
                                             view.color().blueF(),
                                             view.color().alphaF())
                               });
            m_seriesTexs.insert(0, texPtr);
        }
        else {
            seriesProps << SeriesProps {
                offset,
                view.count() + offset,
                view.markerShape(),
                view.markerSize(),
                view.colorType(),
                view.type(),
                0,
                QVector4D(view.color().redF(),
                          view.color().greenF(),
                          view.color().blueF(),
                          view.color().alphaF())
            };
            m_seriesTexs << texPtr;
        }
        offset += view.count();
    }

    m_pointsSize = offset; // Total size.
    m_seriesProps = std::move(seriesProps);

    update();
//    qDebug() << "updateSeries Elapsed Timer: " << ep.elapsed() << offset << QThread::currentThreadId();
}

void GLChartView::updateAxisRange()
{
    m_proj.left = m_axisRange->minX();
    m_proj.right = m_axisRange->maxX();
    m_proj.bottom = m_axisRange->minY();
    m_proj.top = m_axisRange->maxY();

    // Auto Scale if its enable
    m_pvtAutoScale = true;
    if (m_fitWindow) resetProjection();
    autoScaleUpdateAxis();

    emit updateQml();

    update();
}

void GLChartView::updateRectView(const QRect &pRectView)
{
    auto left = normXtoWorld(pRectView.left());
    auto right = normXtoWorld(pRectView.right());
    auto bottom = normYtoWorld(pRectView.bottom());
    auto top = normYtoWorld(pRectView.top());

    m_proj.left = left;
    m_proj.right = right;
    m_proj.bottom = bottom;
    m_proj.top = top;

    //    qDebug() << "Projection: " << m_proj.top << m_proj.left << m_proj.bottom << m_proj.right << pRectView.topLeft() << pRectView.bottomRight();

    emit updateQml();

    update();
}

void GLChartView::preSelect(bool pClear)
{
    if (pClear) {
        memset(m_selectRanges.data(), 0, m_selectRanges.size() * sizeof(QVector4D));
        m_selectRangeIdx = 0;
    }
    m_selectRangeIdx += (qint8)!pClear;

}

void GLChartView::rangeSelecting(const QRect &pRectSelect)
{

    m_selectRanges[m_selectRangeIdx].setX(normYtoWorld(pRectSelect.top()));
    m_selectRanges[m_selectRangeIdx].setY(normXtoWorld(pRectSelect.left()));
    m_selectRanges[m_selectRangeIdx].setZ(normYtoWorld(pRectSelect.bottom()));
    m_selectRanges[m_selectRangeIdx].setW(normXtoWorld(pRectSelect.right()));

    //        qDebug() << "Select Size: " << m_selectRanges.size() << m_selectRangeIdx;
    //        qDebug() << "Select Range is: " << m_selectRange.topLeft() << m_selectRange.bottomRight();

    update();
}

// TODO: Compelete the selectFinished.
void GLChartView::selectFinished() const
{
    //    qDebug() << "on selectFinished(): " << m_selectRanges.size() << m_selectRangeIdx;
    // Running this method inside a other thread
    QThread *invoke = QThread::create([this]() {
        constexpr float OFFSET = 0.12f;
        for (auto & handle : m_series) {
            QVector<int> indices;
            const auto &series = handle->view();
            indices.reserve(series.count());
            for (int i =0 ; i < series.count(); ++i) {
                const PointXYBase &point = series.baseAt(i);
                //            qDebug() << "Selected 0: " << m_selectRanges.constFirst() << offset << point.x() << point.y();
                for (auto &selected: m_selectRanges)
                {
                    if (point.y() <= selected.x() + OFFSET && // top
                        point.x() >= selected.y() - OFFSET && // left
                        point.y() >= selected.z() - OFFSET && // bottom
                        point.x() <= selected.w() + OFFSET) // right
                    {
                        indices << i;
                        break;
                    }
                }
            }
            if (!indices.empty())
                handle->emitselected(indices);
        }
    });
    connect(invoke, &QThread::finished, invoke, [=]() { delete invoke; });

    invoke->start();


}

void GLChartView::adjustView(float mxPixel, float myPixel, float angleDeltaY,
                             bool zoomX, bool zoomY)
{
    // Disabling autoScale until updateAxisRange
    m_pvtAutoScale = false;

    constexpr float zoomFactor = 0.15f;
    float factor = (angleDeltaY > 0) ? (1 - zoomFactor) : (1 + zoomFactor);
    float minX = m_proj.left;
    float maxX = m_proj.right;
    float minY = m_proj.bottom;
    float maxY = m_proj.top;

    float mx = normXtoWorld(mxPixel);
    float my = normYtoWorld(myPixel);

    float newMinX = mx - (mx - minX) * factor;
    float newMaxX = mx + (maxX - mx) * factor;
    float newMinY = my - (my - minY) * factor;
    float newMaxY = my + (maxY - my) * factor;

    if (m_limitView &&
        (newMinX < m_axisRange->minX() ||
         newMaxX > m_axisRange->maxX() ||
         newMinY < m_axisRange->minY() ||
         newMaxY > m_axisRange->maxY()))
    {
        updateAxisRange();
        return;
    }

    if (zoomX) {
        m_proj.left = newMinX;
        m_proj.right = newMaxX;
    }
    if (zoomY) {
        m_proj.bottom = newMinY;
        m_proj.top = newMaxY;
    }

    emit updateQml();

    update();
}

void GLChartView::makeConnections() const
{
    connect(m_bg, &PropertyBackground::changed, this, &GLChartView::update);
    connect(m_axisRange, &PropertyAxisRange::changed, this, &GLChartView::updateAxisRange);
}

void GLChartView::wheelEvent(QWheelEvent *event)
{
    adjustView(event->pos().x(),
               event->pos().y(),
               event->angleDelta().y(),
               true, true);
}

void GLChartView::mousePressEvent(QMouseEvent *event)
{
    // PAN Process
    if (event->button() == panMouseButton && !m_limitView) {
        // Disabling autoScale until updateAxisRange
        m_pvtAutoScale = false;

        m_panning = true;
        m_lastMousePos = event->pos();
        m_panVelocity = QPointF(0, 0);
        // If auto pan after released wasn't finish yet, kill that process.
        if (m_panTimerId != -1) {
            killTimer(m_panTimerId);
            m_panTimerId = -1;
        }
    }
}

void GLChartView::mouseDoubleClickEvent(QMouseEvent *event)
{
    // Reset View to Axis Range
    if (event->button() == panMouseButton) {
        updateAxisRange();
    }
}

void GLChartView::mouseMoveEvent(QMouseEvent *event)
{
    // PAN Process
    if (m_panning) {
        float lastX = normXtoWorld(m_lastMousePos.x());
        float lastY = normYtoWorld(m_lastMousePos.y());
        float curX = normXtoWorld(event->pos().x());
        float curY = normYtoWorld(event->pos().y());

        qreal dt = m_panElaps.restart() / 1000.0;
        QPointF delta((curX - lastX), (curY - lastY));
        m_lastMousePos = event->pos();

        m_proj.left -= delta.x();
        m_proj.right -= delta.x();
        m_proj.bottom -= delta.y();
        m_proj.top -= delta.y();

        m_panVelocity = delta / dt;

        emit updateQml(); // updating labels number values.

        update();
    }

}

void GLChartView::mouseReleaseEvent(QMouseEvent *event)
{
    // PAN Process
    if (event->button() == panMouseButton && !m_limitView) {
        m_panning = false;
        m_panElaps.restart();
        if (m_panTimerId == -1) {
            m_panTimerId = startTimer(16); // Start PAN deceleration ramp timer.
        }
    }

}

void GLChartView::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_panTimerId)
        panAcceleration();

}

void GLChartView::panAcceleration() {

//    constexpr double friction = 0.91; // Velocity Friction.
    qreal dt = m_panElaps.restart() / 1000.0;

    m_proj.left -= m_panVelocity.x() * dt;
    m_proj.right -= m_panVelocity.x() * dt;
    m_proj.bottom -= m_panVelocity.y() * dt;
    m_proj.top -= m_panVelocity.y() * dt;

    m_panVelocity *= velocityCoefficient;

    if (qAbs(m_panVelocity.x()) < 0.03 && qAbs(m_panVelocity.y() < 0.03)) {
        killTimer(m_panTimerId);
        m_panTimerId = -1;
    }

    emit updateQml();
    update();

}
void GLChartView::resetProjection()
{
    /** These condition will also check PolicyHCenter & ... automatically **/
    if (GLutils::hasFlag(GLAutoScalePolicy::PolicyLeft, m_autoScalePolicy)) {
        m_proj.left = qInf();
    }
    if (GLutils::hasFlag(GLAutoScalePolicy::PolicyRight, m_autoScalePolicy)) {
        m_proj.right = qQNaN();
    }
    if (GLutils::hasFlag(GLAutoScalePolicy::PolicyBottom, m_autoScalePolicy)) {
        m_proj.bottom = qInf();
    }
    if (GLutils::hasFlag(GLAutoScalePolicy::PolicyTop, m_autoScalePolicy)) {
        m_proj.top = qQNaN();
    }
}

void GLChartView::autoScaleUpdateAxis()
{
    if (!(m_autoScalePolicy != GLAutoScalePolicy::PolicyNone && m_pvtAutoScale)) return;
    for (auto &handle : m_series) {
        const auto &view = handle->view();
        if (!view.visible()) return;
        autoScaleUpdateAxis(view);
    }

}

void GLChartView::autoScaleUpdateAxis(const GLAbstractSeries &serie)
{
    if (!(m_autoScalePolicy != GLAutoScalePolicy::PolicyNone && m_pvtAutoScale)) return;
    serie.scaleProjection(m_autoScalePolicy, m_proj);
}
