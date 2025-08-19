#ifndef IGLSERIESVIEW_H
#define IGLSERIESVIEW_H

#include <QVariant>
#include "GLStructures.h"

class IGLSeriesView : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(GLSeriesType type READ type CONSTANT)
public:
    enum GLSeriesType {
        SeriesTypeScatter,
        SeriesTypeLine,
        SeriesTypeSpline,
        SeriesTypeArea,
    };
    Q_ENUM(GLSeriesType)

    explicit IGLSeriesView(QObject *parent = nullptr) : QObject{parent} {}
    virtual ~IGLSeriesView() = default;

    // Only what FBO needs
    virtual int size() const noexcept = 0;
    virtual int count() const noexcept = 0;
    virtual int length() const noexcept = 0;
    virtual void reserve(int size) = 0;
    virtual void resize(int size)  = 0;
    virtual bool empty() const noexcept = 0;
    virtual bool isEmpty() const noexcept = 0;
    virtual PointXYBase baseAt(int i) const = 0;

    virtual QVariant makeSelectVariant(const QVector<int> &indices) const = 0;
    virtual int vectorMetaTypeId() const = 0; // qMetaTypeId<QVector<T>>()

    inline QString name() const { return m_name; }
    inline void setName(const QString &name) { m_name = name; emit nameChanged(); }

    inline bool visible() const { return m_visible; }
    inline void setVisible(bool visible) { m_visible = visible; emit visibleChanged(); }

    inline QColor color() const { return m_color; }
    void setColor(const QColor &color) { m_color = color; emit colorChanged(); }

    GLSeriesType type() const { return m_type; }
signals:
    void nameChanged();
    void visibleChanged();
    void colorChanged();
    void selectionChanged(const QVariant &payload, int vectorTypeId);

protected:
    QString m_name;
    bool m_visible = true;
    QColor m_color;
    GLSeriesType m_type;
};


#endif // IGLSERIESVIEW_H
