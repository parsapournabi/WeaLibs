#ifndef GLSERIESSTORAGE_H
#define GLSERIESSTORAGE_H

#include "GLAbstractSeries.h"

/// @brief Template series class based on `GLAbstractSeries`.
/// THe GLSeriesStorage works like a vector, meaning you can add points using methods such as
/// `append`, `replace`, and others.
/// The reason for making this class a template is to provides greater flexibility compared to
/// `QtChart`. In QChart, you can only use `QPointF` as an item, but here you can pass your own custom
/// point data type structs (or simply use the `PointXYBase` struct if you prefer).
///
/// @note T must inherit from `PointXYBase`; otherwise, you will get a compile-time-error.
template<typename T>
class GLSeriesStorage final : public GLAbstractSeries {
    static_assert(std::is_base_of_v<PointXYBase, T>,
                  "ERROR At GLSeriesStorage class: T must inherit from 'PointXYBase' struct!");
public:
    inline void append(const T& point) { m_points.append(point); }
    inline void append(T &&point) { m_points.append(point); }
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

    inline void clear() noexcept { m_points.clear(); }

    inline const T &at(int i) const { return m_points.at(i); }
    inline T &at(int i) { return m_points.at(i); }

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
    /// @brief Most optimize points assignment.
    void replace(const QVector<T> &points) { m_points = std::move(points); }

    /// @brief series points.
    /// @return m_points.
    const QVector<T> &points() const { return m_points; }

    // Overriden methods form GLAbstractSeries
    inline int size() const noexcept override { return m_points.size(); }
    inline int count() const noexcept override { return m_points.size(); }
    inline int length() const noexcept override { return m_points.size(); }
    void reserve(int size) override { m_points.reserve(size); }
    void resize(int size) override { m_points.resize(size); }
    inline bool empty() const noexcept override { return m_points.empty(); }
    inline bool isEmpty() const noexcept override { m_points.isEmpty(); }

protected:
    /** These methods are only used in the GLChartView class. **/
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

    void scaleProjection(GLAutoScalePolicy policy, Projection &proj) const override {
        if (m_points.empty()) return;
        if (isXSorted()) applyScaleWindow(proj, m_points.constFirst(), m_points.constLast(), policy);
        else for (const T &point : m_points) applyScaleWindow(proj, point, point, policy);
    }

    /// @details Converting T to QVariant because a template type cannot be passed through a signal argument.
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
    void applyScaleWindow(Projection &proj,
                          const T &fpoint,
                          const T &lpoint,
                          GLAutoScalePolicy policy) const
    {
        /** These condition will also check PolicyHCenter & ... automatically **/
        if (GLutils::hasFlag(GLAutoScalePolicy::PolicyLeft, policy)) {
            proj.left = qIsInf(proj.left) ? (qreal) fpoint.x() : qMin(proj.left, (qreal) fpoint.x());
        }
        if (GLutils::hasFlag(GLAutoScalePolicy::PolicyRight, policy)) {
            proj.right = qIsNaN(proj.right) ? (qreal) lpoint.x() : qMax(proj.right, (qreal) lpoint.x());
        }
        if (GLutils::hasFlag(GLAutoScalePolicy::PolicyBottom, policy)) {
            proj.bottom = qIsInf(proj.bottom) ? (qreal) fpoint.y() : qMin(proj.bottom, (qreal) fpoint.y());
        }
        if (GLutils::hasFlag(GLAutoScalePolicy::PolicyTop, policy)) {
            proj.top = qIsNaN(proj.top) ? (qreal) lpoint.y() : qMax(proj.top, (qreal) lpoint.y());
        }

    }


    QVector<T> m_points;

};

#endif // GLSERIESSTORAGE_H
