#ifndef IPROPERTIES_H
#define IPROPERTIES_H

#include <QQuickItem>

class IProperties : public QQuickItem {
    Q_OBJECT
public:
    IProperties(QQuickItem *parent = nullptr) : QQuickItem{parent}
    {
        connect(this, &IProperties::changed, this, [=]() { setDataRead(1); });
    }

    bool hasNewData() const { return m_newDataCnt > 0; }
    void setDataRead(int cnt = -1) { m_newDataCnt += cnt; }

signals:
    void changed();

private:
    int m_newDataCnt = 0;
};

#endif // IPROPERTIES_H
