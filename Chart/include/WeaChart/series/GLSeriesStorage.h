#ifndef GLSERIESSTORAGE_H
#define GLSERIESSTORAGE_H

#include "GLAbstractSeries.h"

template<typename T>
class WEACHART_API GLSeriesStorage final : public GLAbstractSeries {
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

    inline const T &at(int i) const { return m_points.at(i); }
    inline T &at(int i) { return m_points[i]; }

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
    GLSeriesStorage &operator=(const QVector<T> &points) { m_points = std::move(points); }

    // Use replace instead of clear + append, it's optimized for performance
    void replace(const QVector<T> &points) { m_points = std::move(points); }

    const QVector<T> &points() const { return m_points; }

    // Overriden methods form GLAbstractSeries
    inline int size() const noexcept override { return m_points.size(); }
    inline int count() const noexcept override { return m_points.size(); }
    inline int length() const noexcept override { return m_points.size(); }
    void reserve(int size) noexcept override { m_points.reserve(size); }
    void resize(int size) override { m_points.resize(size); }
    inline bool empty() const noexcept override { return m_points.empty(); }
    inline bool isEmpty() const noexcept override { m_points.isEmpty(); }

protected:
    PointXYBase baseAt(int i) const override {
        const T& p = m_points.at(i);
        return static_cast<const PointXYBase&>(p);
    }
    PointXYBase *basePoints() const noexcept override {
        static PointXYBase *b_points = nullptr;
        static size_t capacity = 0;
        if (b_points == nullptr) {
            b_points = new PointXYBase[m_points.size()];
            capacity = m_points.size();
        }
        if (capacity < m_points.size()) {
            delete [] b_points;
            b_points = new PointXYBase[m_points.size()];
            capacity = m_points.size();
        }

        const int N = m_points.size();
        const T* src = m_points.data();
        for (int i = 0; i < N; ++i) b_points[i] = src[i];
        return b_points;
    }
    void basePoints(PointXYBase *dst, size_t start) const override {
//        QElapsedTimer ep;
//        ep.start();
        if constexpr (sizeof(PointXYBase) == sizeof(T) || std::is_same_v<T, PointXYBase>) {
            memcpy(dst + start, m_points.data(), m_points.size() * sizeof(PointXYBase));
        }
        else {
            const int N = m_points.size();
            const T* src = m_points.data();
            for (int i = 0; i < N; ++i) dst[i + start] = src[i];
        }
//        qDebug() << "Elapsed timer on memcpy: basePoints: " <<  ep.elapsed();

    }

    QVariant makeSelectVariant(const QVector<int> &indices) const override {
        QVector<T>  sel;
        sel.reserve(indices.size());
        for (auto index : indices)
            if (index >= 0 && index < m_points.size())
                sel.append(m_points[index]);
        sel.squeeze(); // Avoid copy-on-write saving. (This will happen if T has QString field).
        return QVariant::fromValue(sel);
    }

    int vectorMetaTypeId() const override { return qMetaTypeId<QVector<T>>(); }

private:
    QVector<T> m_points;

};

#endif // GLSERIESSTORAGE_H
