#ifndef GLSERIESITEM_H
#define GLSERIESITEM_H

#include "GLAbstractSeries.h"
#include <QQuickItem>
#include <QMetaMethod>
#include <QMetaObject>

class WEACHART_API GLSeriesItem :  public GLAbstractSeries{
    Q_OBJECT
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

    QVariant makeSelectVariant(const QVector<int> &indices) const override {}
    int vectorMetaTypeId() const override {}

    // Properties
    GLAbstractSeries *series() const { return m_series; }
    void setSeries(GLAbstractSeries *series) {
        m_series = series;
        emit seriesChanged();

        if (m_series) {
            synchProperties(this, m_series);
//            synchProperties(m_series, this);
        }
    }

    void synchProperties(QObject *source, QObject* target) {
        const QMetaObject *meta = source->metaObject();
        for (int i(0); i < meta->propertyCount(); ++i) {
            QMetaProperty prop = meta->property(i);
            if (!prop.isReadable() || !prop.isWritable())
                continue;


            QVariant value = prop.read(source);
            prop.write(target, value);

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
    void onPropertyChanged() {
        if (m_updating) return;

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
    bool m_updating = false;
    struct PropertyTarget {
        QMetaProperty prop;
        QObject *target;
    };
    QMap<int, PropertyTarget> m_propertyTargets;

    // Properties
    GLAbstractSeries *m_series = nullptr;

};

#endif // GLSERIESITEM_H
