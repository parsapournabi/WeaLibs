#ifndef GLSERIESSTORAGE_H
#define GLSERIESSTORAGE_H

#include "IGLSeriesView.h"

template<typename T>
class GLSeriesStorage final : public IGLSeriesView {
    static_assert(std::is_base_of_v<PointXYBase, T>,
                  "ERROR At GLSeriesStorage class: T must inherit from 'PointXYBase' struct!");
public:
    inline void append(const T& point) noexcept { m_points << point; }
    inline void append(T &&point) { m_points << point; }
    inline void append(const QVector<T> &points) { m_points += points; }

    inline void insert(int i, T &&point) { m_points.insert(i, point); }
    inline void insert(int i, const T &point) { m_points.insert(i, point); }
    inline void insert(int i, int n, const T &point) { m_points.insert(i, n, point); }

    inline void replace(int i, const T &point) { m_points.replace(i, point); }

    inline void remove(int i) { m_points.remove(i); }
    inline void remove(int i, int n) { m_points.remove(i, n); }

    inline T* data() { return m_points.data(); }
    inline const T* data() const { return m_points.data(); }
    inline const T* constData() const { return m_points.constData(); }

    void clear() { m_points.clear(); }

    const T &at(int i) const { return m_points.at(i); }

    inline T &operator[](int i) { return m_points[i]; }
    inline const T &operator[](int i) const { return m_points[i]; }
    GLSeriesStorage &operator<<(const QVector<T> &points) {
        m_points << points;
        return *this;
    }
    GLSeriesStorage &operator<<(const PointXYBase &point) {
        m_points << point;
        return *this;
    }

    // Optimize Assign
    void replace(const QVector<T> &points) { m_points = std::move(points); }

    const QVector<T> &points() const { return m_points; }

    // Overriden methods form IGLSeriesView
    inline int size() const noexcept override { return m_points.size(); }
    inline int count() const noexcept override { return m_points.size(); }
    inline int length() const noexcept override { return m_points.size(); }
    void reserve(int size) noexcept override { m_points.reserve(size); }
    void resize(int size) override { m_points.resize(size); }
    inline bool empty() const noexcept override { return m_points.empty(); }
    inline bool isEmpty() const noexcept override { m_points.isEmpty(); }

    PointXYBase baseAt(int i) const override {
        const T& p = m_points.at(i);
        return static_cast<const PointXYBase&>(p);
    }

    QVariant makeSelectVariant(const QVector<int> &indices) const override {
        QVector<T> sel;
        sel.reserve(indices.size());
        for (auto index : indices)
            if (index >= 0 && index < m_points.size())
                sel.append(m_points[index]);
        return QVariant::fromValue(sel);
    }

    int vectorMetaTypeId() const override { return qMetaTypeId<QVector<T>>(); }

private:
    QVector<T> m_points;

};

#endif // GLSERIESSTORAGE_H
