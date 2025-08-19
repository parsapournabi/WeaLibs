#include "GLChartview.h"

#include <QRandomGenerator>
#include <QDateTime>
#include <QThread>
#include <QTimer>

GLChartView::GLChartView(QQuickItem *parent) :
    QQuickFramebufferObject{parent},
    m_bg(new PropertyBackground(this)),
    m_axisRange(new PropertyAxisRange(this))
{
    GLChartView::addInstance(this);
    qDebug() << "GLChartView Created" << Instances.size() << this;

    // Initialization
    m_selectRanges.resize(32); // Referer chart.vert
    memset(m_selectRanges.data(), 0, m_selectRanges.size() * sizeof(QVector4D));

    // options
    setMirrorVertically(true);
    setAcceptedMouseButtons(Qt::AllButtons);

    makeConnections();

//    m_timer.start(100, this);

}

GLChartRenderer *GLChartView::createRenderer() const
{
    return new GLChartRenderer();
}

const float GLChartView::normXtoWorld(const float &pX) const
{
    return ((pX - 0) / (width() - 0)) * (m_proj.right - m_proj.left) + m_proj.left;
}

const float GLChartView::normYtoWorld(const float &pY) const
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

const qreal &GLChartView::projLeft() const
{
    return m_proj.left;
}

const qreal &GLChartView::projRight() const
{
    return m_proj.right;
}

const qreal &GLChartView::projBottom() const
{
    return m_proj.bottom;
}

const qreal &GLChartView::projTop() const
{
    return m_proj.top;
}

GLSeriesHandle *GLChartView::addSeries(QSharedPointer<IGLSeriesView> storage)
{
    auto handle = new GLSeriesHandle(storage, this);
    m_series.append(handle);
    return handle;
}

const QVector<GLSeriesHandle *> &GLChartView::series() const
{
    return m_series;
}

void GLChartView::updateSeries()
{
    QVector<PointXYBase> all;
    for (auto &handle : m_series) {
        const auto &view = handle->view();
        for (int i = 0; i < view.count(); ++i) {
            all << view.baseAt(i);
        }
    }
    m_points = std::move(all);

    update();
}

void GLChartView::updateAxisRange()
{
    m_proj.left = m_axisRange->minX();
    m_proj.right = m_axisRange->maxX();
    m_proj.bottom = m_axisRange->minY();
    m_proj.top = m_axisRange->maxY();

    emit projUpdated();

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

//    qDebug() << "Select Size: " << m_selectRanges.size() << m_selectRangeIdx;
//    qDebug() << "Select Range is: " << m_selectRange.topLeft() << m_selectRange.bottomRight();

    update();
}

// TODO: Compelete the selectFinished.
void GLChartView::selectFinished() const
{
    qDebug() << "on selectFinished(): " << m_selectRanges.size() << m_selectRangeIdx;
    for (auto & handle : m_series) {
        QVector<int> indices;
        for (int i =0 ; i < handle->view().count(); ++i) {
            indices << i;
        }
        handle->emitSelectionChanged(indices);
    }
}

void GLChartView::makeConnections() const
{
    connect(m_bg, &PropertyBackground::changed, this, &GLChartView::update);
    connect(m_axisRange, &PropertyAxisRange::changed, this, &GLChartView::updateAxisRange);
}

void GLChartView::wheelEvent(QWheelEvent *event)
{
    constexpr float zoomFactor = 0.15f;
    float angleDelta = event->angleDelta().y();
    float factor = (angleDelta > 0) ? (1 - zoomFactor) : (1 + zoomFactor);
    float minX = m_proj.left;
    float maxX = m_proj.right;
    float minY = m_proj.bottom;
    float maxY = m_proj.top;

    float mx = normXtoWorld(event->position().x());
    float my = normYtoWorld(event->position().x());

    m_proj.left = mx - (mx - minX) * factor;
    m_proj.right = mx + (maxX - mx) * factor;
    m_proj.bottom = my - (my - minY) * factor;
    m_proj.top = my + (maxY - my) * factor;

    emit projUpdated();

    update();
}

void GLChartView::mousePressEvent(QMouseEvent *event)
{
    // Rubber Band (Selection Multi)
    if (event->button() == Qt::LeftButton) {

    }
    // PAN Process
    else if (event->button() == Qt::MiddleButton) {
        m_panning = true;
        m_lastMousePos = event->pos();
        m_panVelocity = QPointF(0, 0);
        if (m_panTimerId != -1) {
            killTimer(m_panTimerId);
            m_panTimerId = -1;
        }
    }
}

void GLChartView::mouseDoubleClickEvent(QMouseEvent *event)
{
    // Reset View to Axis Range
    if (event->button() == Qt::MiddleButton) {
        updateAxisRange();
        return;
    }
}

void GLChartView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
    }
    // PAN Process
    else if (m_panning) {
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

        emit projUpdated();

        update();
    }

}

void GLChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
    }
    // PAN Process
    else if (event->button() == Qt::MiddleButton) {
        m_panning = false;
        m_panElaps.restart();
        if (m_panTimerId == -1) {
            m_panTimerId = startTimer(16);
        }
    }

}

void GLChartView::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timer.timerId()) {
        genPoints();
    }
    if (event->timerId() == m_panTimerId)
        panAcceleration();

}

void GLChartView::panAcceleration() {

    constexpr double friction = 0.91; // Velocity Friction.
    qreal dt = m_panElaps.restart() / 1000.0;

    m_proj.left -= m_panVelocity.x() * dt;
    m_proj.right -= m_panVelocity.x() * dt;
    m_proj.bottom -= m_panVelocity.y() * dt;
    m_proj.top -= m_panVelocity.y() * dt;

    m_panVelocity *= friction;

    if (qAbs(m_panVelocity.x()) < 0.03 && qAbs(m_panVelocity.y() < 0.03)) {
        killTimer(m_panTimerId);
        m_panTimerId = -1;
    }

    emit projUpdated();
    update();

}

void GLChartView::genPoints()
{

}
