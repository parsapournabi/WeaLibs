#ifndef GLSERIESHANDLE_H
#define GLSERIESHANDLE_H

#include <QSharedPointer>
#include "GLAbstractSeries.h"

/*!
  \class GLSeriesHandle
  \brief FBO(GLChartView) handles series read/write via this class.

    @details A class to handling and reading GLSeriesStorage(template class) data from GLChartView FBO.
    Also it can emit GLSeriesStorage selected signal from GLChartView class.
*/
class WEACHART_API GLSeriesHandle : public QObject {
    Q_OBJECT
public:
    explicit GLSeriesHandle(GLAbstractSeries *view, QObject *parent = nullptr) :
        QObject{parent},
        m_view(view) {}

    /// @return series which added on constructure.
    GLAbstractSeries &view() { return *m_view; }
    /// @return series which added on constructure.
    const GLAbstractSeries &view() const { return *m_view; }

    /// @details
    void emitselected(const QVector<int> &indices) {
        if (indices.isEmpty())
            return;
        QVariant payload = m_view->makeSelectVariant(indices);
        const int vectorTypeId = m_view->vectorMetaTypeId();
        emit m_view->selected(payload, vectorTypeId);
    }

private:
    GLAbstractSeries *m_view = nullptr;

};

#endif // GLSERIESHANDLE_H
