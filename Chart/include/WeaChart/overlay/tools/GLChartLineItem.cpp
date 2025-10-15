#include "GLChartLineItem.h"

#include <QSGGeometryNode>
#include <QQuickWindow>


GLChartLineItem::GLChartLineItem(QQuickItem *parent)
    : GLChartItemBase{parent}
{
    setLineStyle(GLStyleLine::SolidLine);
}

qreal GLChartLineItem::lineWidth() const
{
    return m_lineWidth;
}

void GLChartLineItem::setLineWidth(const qreal &width)
{
    if (width == m_lineWidth) return;
    m_lineWidth = width;
    emit lineWidthChanged();
    emit changed();
}

qreal GLChartLineItem::position() const
{
    return m_position;
}

void GLChartLineItem::setPosition(const qreal &pos)
{
    if (pos == m_position) return;
    m_position = pos;

    emit positionChanged();
    emit changed();
}

QVector<QVector2D> GLChartLineItem::vertices() const noexcept
{
    if (isVertical()) {
        if (m_lineWidth <= 1.0)
            return QVector<QVector2D>
                {
                    QVector2D(m_position, 0.0), // p1
                    QVector2D(m_position, 1.0) // p2
                };
        else
            return QVector<QVector2D>
                {
                    QVector2D(m_position - (m_lineWidth * 0.5), 0.0), // p1
                    QVector2D(m_position - (m_lineWidth * 0.5), 1.0), // p2
                    QVector2D(m_position + (m_lineWidth * 0.5), 0.0), // p3
                    QVector2D(m_position + (m_lineWidth * 0.5), 1.0) // p4
                };
    }
    else {

        if (m_lineWidth <= 1.0)
            return QVector<QVector2D>
                {
                    QVector2D(0.0, m_position), // p1
                    QVector2D(1.0, m_position) // p2
                };
        else
            return QVector<QVector2D>
                {
                    QVector2D(0.0, m_position - (m_lineWidth * 0.5)), // p1
                    QVector2D(1.0, m_position - (m_lineWidth * 0.5)), // p2
                    QVector2D(0.0, m_position + (m_lineWidth * 0.5)), // p3
                    QVector2D(1.0, m_position + (m_lineWidth * 0.5)) // p4

                };

    }
}

GLItemType GLChartLineItem::type() const
{
    return GLItemType::Line;
}

GLenum GLChartLineItem::drawType() const
{
    return (m_lineWidth <= 1.0) ? GL_LINES : GL_TRIANGLE_STRIP;
}
