#ifndef QABSTRACTITEMTYPE_H
#define QABSTRACTITEMTYPE_H
#include <QObject>
#include <QDebug>
#include <QVariantList>
#include <qmetaobject.h>

class QAbstractItemBase : public QObject {
    Q_OBJECT
    Q_PROPERTY(int row READ row WRITE setRow NOTIFY dataChanged)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY dataChanged)
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY  dataChanged)
    Q_PROPERTY(QMap<QString, QMap<QString, QVariant>> extraData READ extraData WRITE setExtraData NOTIFY dataChanged) // name: value

public:
    explicit QAbstractItemBase(QObject *parent = nullptr) { setEmpty(true); };

    virtual QVariant getValueByRole(int role) = 0;

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    int row() const { return m_row; }
    void setRow(int row){ m_row = row; }

    bool selected() const { return m_selected; }
    void setSelected(bool row) {
        if (m_selected == row) return;
        m_selected = row;
        emit selectedChanged();
    }

    bool visible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; emit dataChanged(); }

    QMap<QString, QMap<QString, QVariant>> extraData() const { return m_extraData; }
    void setExtraData(QMap<QString, QMap<QString, QVariant>> data) {
        m_extraData = data;
        emit dataChanged();
    }
    void addExtraData(QString name, QMap<QString, QVariant> &value) {
        m_extraData[name] = value;
        qDebug() << " 1 ExtraData: " << extraData();
        emit dataChanged();
    }
    void addExtraData(QString name, QString propName, QVariant value) {
        m_extraData[name][propName] = value;
        qDebug() << "Props: " << name << ' ' << propName << ' ' << value;
        emit dataChanged();
    }

    bool isEmpty() const { return m_empty; }

    Q_INVOKABLE QVariantList getSelfProperties() { return getProperties(false); }

    Q_INVOKABLE QVariantList getParentProperties() { return getProperties(true); }

    Q_INVOKABLE QStringList getSelfPropertiesNames() { return getPropertiesNames(false); }

    Q_INVOKABLE QStringList getParentPropertiesNames() { return getPropertiesNames(true); }

signals:
    void dataChanged();
    void selectedChanged();
    void extraDataChanged();

protected:
    void setEmpty(bool empty) { m_empty = empty; }

private:
    Q_INVOKABLE QVariantList getProperties(bool isParent) {
        QVariantList props_list;
        QMap<QString, QMap<QString, QVariant>> extra_data = extraData();
        const QMetaObject *metaObj;

        if (isParent)
            metaObj = &QAbstractItemBase::staticMetaObject;
        else
            metaObj = metaObject();

        for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); ++i) {
            QMetaProperty prop = metaObj->property(i);
            QVariantMap entry;
            const char *prop_name = prop.name();
//            if (QString(prop_name) == QString("extraData"))
//                continue;

            if (extra_data.contains(prop_name)) {
                QMap<QString, QVariant> data = extra_data[prop_name];
                for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
                    entry[it.key()] = it.value();
                }

            }
            entry["name"] = prop_name;
            entry["value"] = isParent ? -1 : prop.read(this);
            qDebug() << "Entry: " << entry;
            props_list.append(entry);

        }
        return props_list;
    }

    Q_INVOKABLE QStringList getPropertiesNames(bool isParent) {
        QStringList names;
        const QMetaObject *metaObj;

        if (isParent)
            metaObj = &QAbstractItemBase::staticMetaObject;
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
    bool m_visible = true;
    QMap<QString, QMap<QString, QVariant>> m_extraData; // Default properties are visisble, title, columnsWidth

    bool m_empty = false;
    QVariantList m_roles;
};

#endif // QABSTRACTITEMTYPE_H
