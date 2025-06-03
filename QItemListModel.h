#ifndef QITEMTABLEMODEL_H
#define QITEMTABLEMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QtGlobal>
#include <QString>
#include <QMap>
#include <QHash>
#include <QVector>
#include <QDebug>
#include <QVariant>
#include <chrono>


class BaseAbstractListModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit BaseAbstractListModel(QObject *parent = nullptr) :
        QAbstractListModel(parent) {}

    virtual Q_INVOKABLE int insertItem(QObject* itemObj, quint32 targetId) = 0;

    virtual Q_INVOKABLE bool removeItem(quint32 itemId) = 0;
    virtual Q_INVOKABLE bool editItem(int key, QObject* itemObj) = 0;

    virtual Q_INVOKABLE QVariantList getRowData(int row) const = 0;
    virtual Q_INVOKABLE void selectItem(int index) = 0;

    virtual Q_INVOKABLE QVariantList getProperties(bool getParent = true) const = 0;

    virtual Q_INVOKABLE void setProperty(QString name, QString propName, QVariant value) = 0;

};

template<typename ItemType>
class QItemListModel:  public BaseAbstractListModel
{
//    Q_OBJECT
    using clock = std::chrono::high_resolution_clock;
public:
    explicit QItemListModel(QObject* parent = nullptr) :
        BaseAbstractListModel(parent)
    {
        this->roleIdToName = ItemType::getRoles();
        QHashIterator<int, QByteArray> qIt(roleIdToName);
        while (qIt.hasNext())
        {
            qIt.next();
            roleNameToId[QString(qIt.value())] = qIt.key();
        }
    }

    Q_INVOKABLE QHash<int, QByteArray> roleNames() const override
    {
        return roleIdToName;
    }

    Q_INVOKABLE int rowCount(const QModelIndex& parent = QModelIndex()) const override
    {
        return (!parent.isValid()) ? items.count() : 0;
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent)
        int ix =(!parent.isValid()) ? 1 : 0;
        return ix;
    }

    QVariant data(const QModelIndex &index,int role= Qt::DisplayRole) const override {
        if(!index.isValid())
            return {};
        int key = index.row();
        if (items.count() > key) {
            return items[key]->getValueByRole(role);
        }

        return QVariant();
    }


    Q_INVOKABLE int insertItem(QObject* itemObj, quint32 targetId) override
    {
        ItemType *item = qobject_cast<ItemType*>(itemObj);
        if (!item) {
            qWarning() << "Item isn't type of ItemType -> at 'insertItem' ";
        }
        int insertlocation = this->rowCount();
        beginInsertRows(QModelIndex(), insertlocation, insertlocation);
        items.append(item);
        connect(item, &ItemType::dataChanged, this, [=]() {
            int row = items.indexOf(item);
            if (row >= 0)
                safeDataChanged(index(0), index(items.count() - 1));
//                emit dataChanged(index(row), index(row, columnCount() - 1));
            }, Qt::DirectConnection);
        mapItemIdToKey.insert(targetId,items.count()-1);
        mapItemIdToObject.insert(targetId , item);

        // Definition
        item->setId(targetId);
        item->setRow(items.count() - 1);

        // End
        endInsertRows();
        return insertlocation;
    }

    void safeDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
        // Calculating difference between current time & previous emit calling
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - m_nextDataChangeEmit).count();
        if (diff >= DATA_CHANGED_CYCLE) {
//            qDebug() << "Emited";
            emit dataChanged(topLeft, bottomRight);
            m_nextDataChangeEmit = clock::now();
        }

    }

    void clearData(){
        beginResetModel();
        mapItemIdToKey.clear();
        mapItemIdToObject.clear();
        items.clear();
        endResetModel();
    }

    void clear()
    {
        QList<quint32> listItemsSubSystem = mapItemIdToObject.keys();
        for(auto item : listItemsSubSystem)
        {
            removeItem(returnItemTarget(item),item);
        }

    }

    int countItem()
    {
        return items.count();
    }

    Q_INVOKABLE bool editItem(int key, QObject* itemObj) override
    {
        ItemType *item = qobject_cast<ItemType*>(itemObj);
        if (!item) {
            qWarning() << "Item isn't type of ItemType -> at 'editItem' ";
        }
        if (items.count() > key)
        {
            items[key] = item;
            /** Set dataChanged inside of your QItemType class.**/
        }
        return false;
    }

    Q_INVOKABLE QVariantList getProperties(bool getParent = true) const override {
        if (items.isEmpty())
            return QVariantList();
        return getParent ? items[0]->getParentProperties() + items[0]->getSelfProperties() : items[0]->getSelfProperties();
    }

    Q_INVOKABLE void setProperty(QString name, QString propName, QVariant value) override {
        if (items.isEmpty()) {
            qDebug() << "Items is empty while on setProperty";
            return;
        }
        items[0]->addExtraData(name, propName, value);

    }

    Q_INVOKABLE bool removeItem(quint32 itemId) override
    {
        //  qint32 index = getIndex(key);

        if(mapItemIdToObject.contains(itemId))
            return removeItem(mapItemIdToObject[itemId],itemId);

        return false;
    }

    ItemType* getItem(int index)
    {
        if (items.count() > index)
        {
            return items.value(index);
        }

        return nullptr;
    }

    int getItemKey(ItemType* item)
    {
        if (items.contains(item))
        {
            return items.indexOf(item);
        }
        return -1;
    }

    Q_INVOKABLE QVariantList getRowData(int row) const override
    {
        QVariantList rowData;
        for (auto &role: roleNameToId)
            rowData.append(data(index(row), role));
        return rowData;
    }

    Q_INVOKABLE void selectItem(int index) override {
        qDebug() << "Select row: " << index;
        for (int i = 0; i < items.size(); ++i)
            items[i]->setSelected(i == index);
        emit dataChanged(this->index(index), this->index(index));
        if (m_selectedIndex != -1)
            emit dataChanged(this->index(m_selectedIndex), this->index(m_selectedIndex));
        m_selectedIndex = index;
//        safeDataChanged(this->index(0), this->index(items.count() - 1));
    }

    int getItemIndex(ItemType* item)
    {
        if (items.contains(item))
        {
            return items.indexOf(item);
        }
        return -1;
    }

    int getIndex(quint32 key){
        if(mapItemIdToKey.contains(key))
            return mapItemIdToKey[key];

        return -1;
    }

    bool validItemId(quint32 trgId){
        if(mapItemIdToKey.contains(trgId))
            return true;

        return false;

    }
    bool validItemObject(quint32 trgId){
        if(mapItemIdToObject.contains(trgId))
            return true;

        return false;

    }

    ItemType * returnItemTarget(quint32 targetId)
    {
        ItemType * item = nullptr;
        if(mapItemIdToObject.contains(targetId))
        {
            item = mapItemIdToObject[targetId];
        }
        return item;
    }

private:

    // Overload method
    bool removeItem(ItemType* item, quint32 key)
    {
        if(items.contains(item)){
            int index = items.indexOf(item);
            beginRemoveRows(QModelIndex(), index, index);

            items.removeAt(index);
            mapItemIdToKey.remove(key);
            mapItemIdToObject.remove(key);
            setRowNumbers();

            endRemoveRows();
            return true;
        }
        return false;
    }

    void setRowNumbers() {

        // Redefining row Numbers
        for (int i = 0; i < items.count(); ++i) {
            items[i]->setRow(i);
        }
    }



protected:
    QVector<ItemType*> items;

public:
    const int DATA_CHANGED_CYCLE = 200; // ms, UI update cycle

private:

    QHash<QString, int> roleNameToId;
    QHash<int, QByteArray> roleIdToName;
    QMap <quint32 , quint32>mapItemIdToKey;// trgId--->index item
    QHash<quint32 , ItemType*>mapItemIdToObject;//targetId , object

    int m_selectedIndex = -1; // Latest index selected

    std::chrono::time_point<clock> m_nextDataChangeEmit = clock::now();

};

#endif // QITEMTABLEMODEL_H
