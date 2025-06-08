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
#include <QTimerEvent>
#include <chrono>


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

template<typename ItemType>
class QItemListModel:  public QItemListModelBase
{
//    Q_OBJECT
    using clock = std::chrono::high_resolution_clock;
public:
    explicit QItemListModel(QObject* parent = nullptr) :
        QItemListModelBase(parent)
    {
        // Initialization
        update();
        m_timerId = startTimer(REFRESH_INTERVAL);

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

    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return items.count();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent)
        return !parent.isValid() ? 1 : 0;

    }

    QVariant data(const QModelIndex &index,int role= Qt::DisplayRole) const override {
        if(!index.isValid())
            return {};
        int key = index.row();
        if (items.count() > key) {
//            if (role == Qt::DisplayRole)
                // Default Filtering column
            return items[key]->getValueByRole(role);
        }

        return QVariant();
    }


    Q_INVOKABLE int insertItem(QObject* itemObj, quint32 itemId) override
    {
        ItemType *item = qobject_cast<ItemType*>(itemObj);
        if (!item) {
            qWarning() << "Item isn't type of ItemType -> at 'insertItem' ";
        }
        int insertlocation = this->rowCount();
        beginInsertRows(QModelIndex(), insertlocation, insertlocation);
        items.append(item);
        mapItemIdToRow.insert(itemId,items.count()-1);
        mapItemIdToObject.insert(itemId , item);

        // Definition
        item->setId(itemId);
        item->setRow(items.count() - 1);

        // End
        endInsertRows();
        return insertlocation;
    }

    Q_INVOKABLE bool removeItem(quint32 itemId) override
    {
        if(mapItemIdToObject.contains(itemId))
            return removeItem(mapItemIdToObject[itemId],itemId);

        return false;
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
            return true;
        }
        return false;
    }

    Q_INVOKABLE bool editItemById(int itemId, QObject* itemObj)
    {
        ItemType *item = qobject_cast<ItemType*>(itemObj);
        if (!item) {
            qWarning() << "Item isn't type of ItemType -> at 'editItem' ";
            return false;
        }
        if (!mapItemIdToObject.contains(itemId)) {
            qDebug() << "item Id not exists at mapItemIdToObjedct!" << itemId;
            return false;
        }
        mapItemIdToObject[itemId] = item;

        return true;
    }

    Q_INVOKABLE QVariantList getHeadersData(bool getParent = true) const override {
        if (items.isEmpty()) {
            ItemType item;
//            return QVariantList();
            return getParent ? item.getParentProperties() + item.getSelfProperties() : item.getSelfProperties();
        }
        return getParent ? items[0]->getParentProperties() + items[0]->getSelfProperties() : items[0]->getSelfProperties();
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
        update();
        if (m_selectedIndex != -1)
            update();
        m_selectedIndex = index;
    }

    ItemType * returnItemObject(quint32 itemId)
    {
        ItemType * item = nullptr;
        if(mapItemIdToObject.contains(itemId))
        {
            item = mapItemIdToObject[itemId];
        }
        return item;
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

    int getItemIndex(ItemType* item)
    {
        if (items.contains(item))
        {
            return items.indexOf(item);
        }
        return -1;
    }

    int getIndex(quint32 key){
        if(mapItemIdToRow.contains(key))
            return mapItemIdToRow[key];

        return -1;
    }

    bool validItemId(quint32 trgId){
        if(mapItemIdToRow.contains(trgId))
            return true;

        return false;

    }

    bool validItemObject(quint32 trgId){
        if(mapItemIdToObject.contains(trgId))
            return true;

        return false;

    }

    void update(int topLeft = 0, int topRight = -1) {
//        beginResetModel();
//        endResetModel();
        emit dataChanged(index(topLeft), index(topRight == -1 ? items.count() - 1 : topRight));

    }

    QList<unsigned int> returnItemIds() const {
        return mapItemIdToRow.keys();
    }

    int countItem() const {
        return items.count();
    }

    bool isEmpty() const {
        return items.isEmpty();
    }

    void clearData() {
        beginResetModel();
        mapItemIdToRow.clear();
        mapItemIdToObject.clear();
        items.clear();
        endResetModel();
    }

    void clear() {
        QList<quint32> listItemsSubSystem = mapItemIdToObject.keys();
        for(auto item : listItemsSubSystem)
        {
            removeItem(returnItemObject(item),item);
        }

    }
private:

    // Overload method
    bool removeItem(ItemType* item, quint32 key)
    {
        if(items.contains(item)){
            int index = items.indexOf(item);
            beginRemoveRows(QModelIndex(), index, index);

            items.removeAt(index);
            mapItemIdToRow.remove(key);
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

    void timerEvent(QTimerEvent *event) override {
        if (event->timerId() == m_timerId)
            update();
    }

private:
    QHash<QString, int> roleNameToId;
    QHash<int, QByteArray> roleIdToName;
    QVector<ItemType*> items;
    QMap <quint32 , quint32>mapItemIdToRow;// trgId--->index item
    QHash<quint32 , ItemType*>mapItemIdToObject;//itemId , object

    int m_timerId;
    int m_selectedIndex = -1; // Latest index selected

    std::chrono::time_point<clock> m_nextDataChangeEmit = clock::now();

    const int REFRESH_INTERVAL = 200; // ms, UI update cycle

};

#endif // QITEMTABLEMODEL_H
