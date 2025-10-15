#include "GLChartGateItem.h"

GLChartGateItem::GLChartGateItem(QQuickItem *parent)
    : GLChartItemBase{parent}
{

}

qreal GLChartGateItem::from() const
{
    return m_from;
}

void GLChartGateItem::setFrom(const qreal &pos)
{
    if (qFuzzyCompare(pos, m_from)) return;
    m_from = pos;
    emit fromChanged();
    emit changed();
}

qreal GLChartGateItem::to() const
{
    return m_to;
}

void GLChartGateItem::setTo(const qreal &pos)
{
    if (qFuzzyCompare(pos, m_from)) return;
    m_to = pos;
    emit toChanged();
    emit changed();
}

QVector<QVector2D> GLChartGateItem::vertices() const noexcept
{
    if (isVertical()) {
        return QVector<QVector2D>
            {
                QVector2D(m_from, 0.0),
                QVector2D(m_from, 1.0),
                QVector2D(m_to, 0.0),
                QVector2D(m_to, 1.0)
            };
    }
    else {
        return QVector<QVector2D>
            {
                QVector2D(0.0, m_from),
                QVector2D(1.0, m_from),
                QVector2D(0.0, m_to),
                QVector2D(1.0, m_to)
            };

    }
}

GLItemType GLChartGateItem::type() const
{
    return GLItemType::Gate;
}

GLenum GLChartGateItem::drawType() const
{
    return GL_TRIANGLE_STRIP;
}
