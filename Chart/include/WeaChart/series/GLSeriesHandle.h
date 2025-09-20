#ifndef GLSERIESHANDLE_H
#define GLSERIESHANDLE_H

#include <QSharedPointer>
#include "GLAbstractSeries.h"

class WEACHART_API GLSeriesHandle : public QObject {
    Q_OBJECT
public:
    explicit GLSeriesHandle(QSharedPointer<GLAbstractSeries> view, QObject *parent = nullptr) :
        QObject{parent},
        m_view(std::move(view)) {}

    GLAbstractSeries &view() { return *m_view; }
    const GLAbstractSeries &view() const { return *m_view; }

    void emitselected(const QVector<int> &indices) {
        if (indices.isEmpty())
            return;
        QVariant payload = m_view->makeSelectVariant(indices);
        const int vectorTypeId = m_view->vectorMetaTypeId();
        emit m_view->selected(payload, vectorTypeId);
    }

private:
    QSharedPointer<GLAbstractSeries> m_view;

};

#endif // GLSERIESHANDLE_H
