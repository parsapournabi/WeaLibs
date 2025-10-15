#ifndef GLABSTRACTSERIES_H
#define GLABSTRACTSERIES_H

#include <QVariant>
#include <QImage>
#include "WeaChart/utils/GLStructures.h"
#include "WeaChart/properties/PropertySeries.h"

/// @brief Abstract series class (it can't be create as object).
/// Creation idea of this class is:
/// Because the GLSeriesStorage is a template class and it can't inherit from Qt Objects.
/// GLSeriesStorage can't have any signal or  emit any signal.
/// So this `GLAbstractSeries` class created to `GLSeriesStorage` class inherit from it.
/// Now `GLSeriesStorage` class can have signals and also can have some method to emit signals. (like requestUpdateSeriesData())
/// @note All methods are virtual required (that means it must override at inherited class.)
class WEACHART_API GLAbstractSeries : public PropertySeries {
    Q_OBJECT
public:
    explicit GLAbstractSeries(QQuickItem *parent = nullptr) : PropertySeries{parent} {}
    virtual ~GLAbstractSeries() = default;

    // Only what FBO needs
    /// @brief size of the series points.
    virtual int size() const noexcept = 0;
    /// @brief same as size().
    virtual int count() const noexcept = 0;
    /// @brief same as size().
    virtual int length() const noexcept = 0;
    /// @brief Reserving the series poitns vector.
    virtual void reserve(int size) = 0;
    /// @brief Resizeing the series points vector.
    virtual void resize(int size)  = 0;
    /// @return true if the series points vector is empty.
    virtual bool empty() const noexcept = 0;
    /// @brief same as empty().
    virtual bool isEmpty() const noexcept = 0;
    /// @brief Works like the at() in the QVector.
    /// @return casted T from the series points vector.
    virtual PointXYBase baseAt(int i) const = 0;
    /// @brief The casted series points vector into the PointXYBase C array.
    /// @return The series points vector as the C array.
    virtual PointXYBase *basePoints() const = 0;
    /// @brief Same as the basePoints() but the result will set into the dst param.
    /// @param dst: The destinition of the casted series points vector.
    /// @param start: The Start index of the dst param.
    virtual void basePoints(PointXYBase *dst, size_t start) const = 0;
    /// @brief Modifying GLChartView projection if the fitWindow & autoScalePolicy properties are enable.
    /// @param GLAtuoScalePolicy: policy (scaling type).
    /// @param Projection: proj, a reference that comes from GLChartView::m_proj
    virtual void scaleProjection(GLAutoScalePolicy policy, Projection &proj) const = 0;
    /// @brief Locating the selected series points.
    /// @param The indices of selected points.
    /// @return casted QVector<T> to the QVariant.
    virtual QVariant makeSelectVariant(const QVector<int> &indices) const = 0;
    /// @return QVector<T> metaTypeId.
    virtual int vectorMetaTypeId() const = 0; // qMetaTypeId<QVector<T>>()

signals:
    /// @brief This signal is connected to the GLChartView::updateSeries slot.
    void signalUpdateSeriesData();
    /// @brief After the makeSelectVariant the selected signal will emit.
    /// @param payload: The QVariant that you should cast it to the QVector<T>.
    /// @param vectorTypeId: refers vectorMetaTypeId().
    void selected(const QVariant &payload, int vectorTypeId);

public slots:
    /// @brief Emitting the signalUpdateSeriesData()
    void requestUpdateSeriesData() { emit signalUpdateSeriesData(); }
};


#endif // GLABSTRACTSERIES_H
