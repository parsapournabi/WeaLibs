#ifndef QITEMLISTMODDELBASE_H
#define QITEMLISTMODDELBASE_H

#include <QObject>
#include <QAbstractListModel>
#include <QtGlobal>
#include <QString>
#include <QMap>
#include <QHash>
#include <QVector>
#include <QDebug>
#include <QVariant>


class QItemListModelBase : public QAbstractListModel {
    Q_OBJECT
public:
    explicit QItemListModelBase(QObject *parent = nullptr) :
        QAbstractListModel(parent) {}

    virtual Q_INVOKABLE int insertItem(QObject* itemObj, quint32 itemId) = 0;

    virtual Q_INVOKABLE bool removeItem(quint32 itemId) = 0;
    virtual Q_INVOKABLE bool editItem(int key, QObject* itemObj) = 0;

    virtual Q_INVOKABLE QVariantList getRowData(int row) const = 0;
    virtual Q_INVOKABLE void selectItem(int index) = 0;

    virtual Q_INVOKABLE QVariantList getHeadersData(bool getParent = true) const = 0;

};

#endif // QITEMLISTMODDELBASE_H
