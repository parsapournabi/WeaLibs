#ifndef IPROPERTIES_H
#define IPROPERTIES_H

#include <QQuickItem>
#include "WeaChart/WeaChart_export.h"

/// @brief Abstract QML/C++ Property class.
/// @details Don't need to write changed signal for each Property class.
class WEACHART_API IProperties : public QQuickItem {
    Q_OBJECT
public:
    IProperties(QQuickItem *parent = nullptr) : QQuickItem{parent}
    {
        connect(this, &IProperties::changed, this, [=]() { setDataRead(1); });
    }

    /// @return true if m_newDataCnt > 0.
    bool hasNewData() const { return m_newDataCnt > 0; }
    /// @brief Decreasing the read data.
    /// @param Number of the data that has been read.
    void setDataRead(int cnt = -1) { m_newDataCnt += cnt; }

signals:
    void changed();

private:
    /// @brief Makes newDataEmitted queue by using the counter.
    int m_newDataCnt = 0;
};

#endif // IPROPERTIES_H
