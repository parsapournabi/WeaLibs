#ifndef QITEMBASE_H
#define QITEMBASE_H

#include <QObject>
#include <QDebug>
#include <QVariantList>
#include <qmetaobject.h>

class QItemBase : public QObject {
    Q_OBJECT

public:
    explicit QItemBase(QObject *parent = nullptr) { setEmpty(true); };

    virtual QVariant getValueByRole(int role) = 0;
    virtual QString getTitleByRole(int role) const = 0;
    virtual QVariantMap getHeaderDataByRole(int role) const = 0;

    int id() const { return m_id; }
    void setId(int id) {
        if (m_id == id)
            return;
        m_id = id;
        setUpdated(true);
    }

    int row() const { return m_row; }
    void setRow(int row){
        if (m_row == row)
            return;
        m_row = row;
        setUpdated(true);
    }

    bool itemSelected() const { return m_itemSelected; }
    void setItemSelected(bool selected) {
        if (m_itemSelected == selected)
            return;
        m_itemSelected = selected;
        setUpdated(true);
    }

    bool isEmpty() const { return m_empty; }

    bool hasUpdated() const { return m_hasUpdated; }

    void setUpdated(const bool updated) { m_hasUpdated = updated; }

protected:
    void setEmpty(bool empty) { m_empty = empty; }

private:
    int m_row = 0;
    int m_id = -1; // -1 means Item has no Id!
    bool m_itemSelected = false;
    bool m_hasUpdated = false;

    bool m_empty = false;
    QVariantList m_roles;
};

#endif // QITEMBASE_H
