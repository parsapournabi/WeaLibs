#ifndef GLSERIESITEM_H
#define GLSERIESITEM_H

#include "GLAbstractSeries.h"
#include <QQuickItem>
#include <QMetaMethod>
#include <QMetaObject>

/// @qmlclass GLSeriesItem
/// @brief GLSeriesStorage class as QML Component.
/// @details This class is used n QML SeriesItem. It is similar to the `GLSeriesStorage` class but has some differences.
/// @note The `series` property is a required property that must be set in QML.
/// By setting the `series` property, all signals and properties of the series object will synchronized with
/// this class's signals/properties.
class WEACHART_API GLSeriesItem :  public GLAbstractSeries{
    Q_OBJECT
    /// @brief A required property that must be assign in QML.
    Q_PROPERTY(GLAbstractSeries *series READ series WRITE setSeries NOTIFY seriesChanged)
public:
    GLSeriesItem(QQuickItem *parent = nullptr) : GLAbstractSeries{parent} {}
    int size() const noexcept override {}
    int count() const noexcept override {}
    int length() const noexcept override {}
    void reserve(int size) override {}
    void resize(int size)  override {}
    bool empty() const noexcept override {}
    bool isEmpty() const noexcept override {}
    PointXYBase baseAt(int i) const override {}
    PointXYBase *basePoints() const override {}
    void basePoints(PointXYBase *dst, size_t start) const override {}
    void scaleProjection(GLAutoScalePolicy policy, Projection &proj) const override {}

    QVariant makeSelectVariant(const QVector<int> &indices) const override {}
    int vectorMetaTypeId() const override {}

    // Properties
    /// @brief getter of the series property.
    GLAbstractSeries *series() const { return m_series; }
    /// @brief setter of the series property.
    void setSeries(GLAbstractSeries *series) {
        m_series = series;
        emit seriesChanged();

        if (m_series) {
            synchProperties(this, m_series);
//            synchProperties(m_series, this);
        }
    }

    /// @brief series property & this class, properties synchronization
    /// @details Synchronizing series->signals/properties connections and values with this class.
    void synchProperties(QObject *source, QObject* target) {
        const QMetaObject *meta = source->metaObject();
        // Reading this class all Q_PROPERTY. (Referer PropertySeries.h0
        for (int i(0); i < meta->propertyCount(); ++i) {
            QMetaProperty prop = meta->property(i);
            if (!prop.isReadable() || !prop.isWritable())
                continue;


            QVariant value = prop.read(source);
            prop.write(target, value); // Writing current class prop value to target class property.

            // If property has a signal.
            if (prop.hasNotifySignal()) {
                int sigIndex = prop.notifySignalIndex();
                m_propertyTargets[sigIndex] = {prop, target};
                QMetaMethod signal = prop.notifySignal();
                QMetaObject::Connection conn = QObject::connect(source,
                                                                signal,
                                                                this,
                                                                metaObject()->method(metaObject()->indexOfSlot("onPropertyChanged()")));
                Q_UNUSED(conn);
            }
        }
    }

signals:
    void seriesChanged();

private slots:
    /// @details Updating target property value when property's signal notified.
    void onPropertyChanged() {
        if (m_updating) return; // Avoid race condition & duplicate assignment.

        QObject *senderObj = sender();
        int sigIndex = senderSignalIndex();

        auto it = m_propertyTargets.find(sigIndex);
        if (it == m_propertyTargets.end()) return;

        QMetaProperty prop = it->prop;
        QObject *target = it->target;

        QVariant value = prop.read(senderObj);

        m_updating = true;
        prop.write(target, value);
        m_updating = false;

    }

private:
    /// @brief Avoids duplicate property assignment.
    bool m_updating = false;
    /// @brief A private struct which used on GLSeriesItem as QMap value.
    struct PropertyTarget {
        QMetaProperty prop;
        QObject *target;
    };
    /// @brief key = Property/Signal index. value = {prop, target}
    QMap<int, PropertyTarget> m_propertyTargets;

    // Properties
    /// @brief Member of the series property.
    GLAbstractSeries *m_series = nullptr;

};

#endif // GLSERIESITEM_H
