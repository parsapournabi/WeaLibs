#ifndef GLCHARTGATEITEM_H
#define GLCHARTGATEITEM_H

#include "GLChartItemBase.h"

class GLChartGateItem : public GLChartItemBase
{
    Q_OBJECT
    Q_PROPERTY(qreal from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(qreal to READ to WRITE setTo NOTIFY toChanged)
public:
    explicit GLChartGateItem(QQuickItem *parent = nullptr);

    qreal from() const;
    void setFrom(const qreal &pos);

    qreal to() const;
    void setTo(const qreal &pos);

signals:
    void fromChanged();
    void toChanged();
protected:
    QVector<QVector2D> vertices() const noexcept override;
    GLItemType type() const override;
    GLenum drawType() const override;

private:
    qreal m_from;
    qreal m_to;

};

#endif // GLCHARTGATEITEM_H
