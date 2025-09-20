#ifndef GLABSTRACTSERIES_H
#define GLABSTRACTSERIES_H

#include <QVariant>
#include <QImage>
#include "WeaChart/utils/GLStructures.h"
#include "WeaChart/properties/PropertySeries.h"

class WEACHART_API GLAbstractSeries : public PropertySeries {
    Q_OBJECT
public:
    explicit GLAbstractSeries(QQuickItem *parent = nullptr) : PropertySeries{parent} {}
    virtual ~GLAbstractSeries() = default;

    // Only what FBO needs
    virtual int size() const noexcept = 0;
    virtual int count() const noexcept = 0;
    virtual int length() const noexcept = 0;
    virtual void reserve(int size) = 0;
    virtual void resize(int size)  = 0;
    virtual bool empty() const noexcept = 0;
    virtual bool isEmpty() const noexcept = 0;
    virtual PointXYBase baseAt(int i) const = 0;
    virtual PointXYBase *basePoints() const = 0;
    virtual void basePoints(PointXYBase *dst, size_t start) const = 0;

    virtual QVariant makeSelectVariant(const QVector<int> &indices) const = 0;
    virtual int vectorMetaTypeId() const = 0; // qMetaTypeId<QVector<T>>()

signals:
    void signalUpdateSeriesData();

public slots:
    void requestUpdateSeriesData() { emit signalUpdateSeriesData(); }
};


#endif // GLABSTRACTSERIES_H
