#ifndef QITEMBASE_H
#define QITEMBASE_H

#include <QObject>
#include <QDebug>
#include <QVariantList>
#include <qmetaobject.h>

class QItemBase : public QObject {
    Q_OBJECT
    Q_PROPERTY(int row READ row WRITE setRow CONSTANT)
    Q_PROPERTY(int id READ id WRITE setId CONSTANT)
    Q_PROPERTY(bool selected READ selected WRITE setSelected CONSTANT)

public:
    explicit QItemBase(QObject *parent = nullptr) { setEmpty(true); };

    virtual QVariant getValueByRole(int role) = 0;
    virtual int columnCount() {
        return getProperties(false).length() + getProperties(true).length();
    }

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    int row() const { return m_row; }
    void setRow(int row){ m_row = row; }

    bool selected() const { return m_selected; }
    void setSelected(bool row) {
        if (m_selected == row) return;
        m_selected = row;
    }

    bool isEmpty() const { return m_empty; }

    Q_INVOKABLE QVariantList getSelfProperties() { return getProperties(false); }

    Q_INVOKABLE QVariantList getParentProperties() { return getProperties(true); }

    Q_INVOKABLE QStringList getSelfPropertiesNames() { return getPropertiesNames(false); }

    Q_INVOKABLE QStringList getParentPropertiesNames() { return getPropertiesNames(true); }

protected:
    void setEmpty(bool empty) { m_empty = empty; }

private:
    Q_INVOKABLE QVariantList getProperties(bool isParent) {
        QVariantList props_list;
        const QMetaObject *metaObj;

        if (isParent)
            metaObj = &QItemBase::staticMetaObject;
        else
            metaObj = metaObject();

        for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); ++i) {
            QMetaProperty prop = metaObj->property(i);
            QVariantMap entry;
            const char *prop_name = prop.name();

            entry["name"] = prop_name;
            entry["title"] = prop_name;
            entry["visible"] = true;
//            entry["value"] = isParent ? -1 : prop.read(this);
//            qDebug() << "Entry: " << entry;
            props_list.append(entry);
        }
        return props_list;
    }

    Q_INVOKABLE QStringList getPropertiesNames(bool isParent) {
        QStringList names;
        const QMetaObject *metaObj;

        if (isParent)
            metaObj = &QItemBase::staticMetaObject;
        else
            metaObj = metaObject();

        for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); ++i) {
            QMetaProperty prop = metaObj->property(i);
            names.append(prop.name());
        }
        return names;
    }

private:
    int m_row = 0;
    int m_id = -1; // -1 means Item has no Id!
    bool m_selected = false;

    bool m_empty = false;
    QVariantList m_roles;
};

#endif // QITEMBASE_H
