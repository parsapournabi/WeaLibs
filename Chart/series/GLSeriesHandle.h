#ifndef GLSERIESHANDLE_H
#define GLSERIESHANDLE_H

#include <QSharedPointer>
#include "IGLSeriesView.h"

class GLSeriesHandle : public QObject {
    Q_OBJECT
public:
    explicit GLSeriesHandle(QSharedPointer<IGLSeriesView> view, QObject *parent = nullptr) :
        QObject{parent},
        m_view(std::move(view)) {}

    IGLSeriesView &view() { return *m_view; }
    const IGLSeriesView &view() const { return *m_view; }

    void emitSelectionChanged(const QVector<int> &indices) {
        QVariant payload = m_view->makeSelectVariant(indices);
        const int vectorTypeId = m_view->vectorMetaTypeId();
//        emit selectionChanged(payload, vectorTypeId);
        emit m_view->selectionChanged(payload, vectorTypeId);
    }

signals:
    // Generic signal (works with any T once its QVector<T> is registered)
    void selectionChanged(const QVariant &payload, int vectorTypeId);

private:
    QSharedPointer<IGLSeriesView> m_view;

};

#endif // GLSERIESHANDLE_H
