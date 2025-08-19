#ifndef IPROPERTIES_H
#define IPROPERTIES_H

#include <QObject>

class IProperties : public QObject {
    Q_OBJECT
public:
    IProperties(QObject *parent = nullptr) : QObject{parent}
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
