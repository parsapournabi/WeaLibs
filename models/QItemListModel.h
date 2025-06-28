#ifndef QITEMTABLEMODEL_H
#define QITEMTABLEMODEL_H

#include <QTimerEvent>
#include <QMetaObject>
#include <QMetaEnum>
#include <chrono>
#include "IQItemListModel.h"

template<typename ItemType>
class QItemListModel:  public IQItemListModel
{
//    Q_OBJECT
public:

    explicit QItemListModel(QObject* parent = nullptr) :
        IQItemListModel(parent)
    {
        // Initialization
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

    Q_INVOKABLE int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent)
        return !parent.isValid() ? 1 : 0;
//        qDebug() << "ColumnsCount" << roleIdToName.count();
//        return roleIdToName.count();

    }

    Q_INVOKABLE QVariant data(const QModelIndex &index,int role= Qt::DisplayRole) const override {
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

    Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override{
        const QMetaObject *metaObj = &ItemType::staticMetaObject;
        if (!metaObj->enumeratorCount()) {
            qCritical() << "QItemBase Child class must include Q_ENUM macro for Roles";
            return QVariant();
        }
        QMetaEnum enm = metaObj->enumerator(0);
        if (role == Qt::DisplayRole) // Entered by Section
            role = Qt::UserRole + 1 + section; // Locating for specific role...
        ItemType item;
        return item.getHeaderDataByRole(role);
    }

    Q_INVOKABLE QVariantList headersData() const override {
        const QMetaObject *metaObj = &ItemType::staticMetaObject;
        if (!metaObj->enumeratorCount()) {
            qCritical() << "QItemBase Child class must include Q_ENUM macro for Roles";
            return QVariantList();
        }
        QMetaEnum enm = metaObj->enumerator(0);
        QVariantList headers;
        ItemType item;
        for (int i = 0; i < enm.keyCount(); ++i) {
            int enm_role = enm.value(i);
            qDebug() << "Prop Name" << enm.key(i) << enm_role;
            QVariantMap title = item.getHeaderDataByRole(enm_role);
            title["name"] = QString(roleIdToName[enm_role]);
            headers.append(title);
        }
        return headers;
    }

    Q_INVOKABLE QVariantList headersTitle() const override {
        const QMetaObject *metaObj = &ItemType::staticMetaObject;
        if (!metaObj->enumeratorCount()) {
            qCritical() << "QItemBase Child class must include Q_ENUM macro for Roles";
            return QVariantList();
        }
        QMetaEnum enm = metaObj->enumerator(0);
        QVariantList headers;
        ItemType item;
        for (int i = 0; i < enm.keyCount(); ++i) {
            int enm_role = enm.value(i);
            qDebug() << "Prop Name" << enm.key(i) << enm_role;
            QString title = item.getTitleByRole(enm_role);
            headers.append(title);
        }
        return headers;
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
            items[i]->setItemSelected(i == index);
        updateAll();
        m_selectedIndex = index;
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
            item->setUpdated(false);
            emit dataChanged(index(key), index(key));
            return true;
        }
        return false;
    }

    Q_INVOKABLE bool editItemById(int itemId, QObject* itemObj) override
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
        int row = mapItemIdToRow[itemId];
        items[row] = item;
        item->setUpdated(false);
        emit dataChanged(index(row), index(row));
//        mapItemIdToObject[itemId] = item;
        return true;
    }

    ItemType *returnItemObject(quint32 itemId)
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

    void updateAll() {
        if (!items.isEmpty())
            emit dataChanged(index(0), index(items.count() - 1));

    }

    void updateChangedItems() {
        if (items.isEmpty())
            return;
        QVector<int> changedItems;
        for (int i = 0; i < items.count(); ++i) {
            if (items[i]->hasUpdated()) {
                changedItems.append(i);
                items[i]->setUpdated(false);
            }

        }
        if (changedItems.isEmpty())
            return;
        auto minMax = std::minmax_element(changedItems.begin(), changedItems.end());
        int minRow = *minMax.first;
        int maxRow = *minMax.second;
        emit dataChanged(index(minRow), index(maxRow));

    }

    QList<unsigned int> returnItemIds() const {
        return mapItemIdToRow.keys();
    }

    bool isEmpty() const {
        return items.isEmpty();
    }

    bool autoRefresh() const { return m_autoRefresh; }

    void setAutoRefresh(bool enable, bool only_changed_items = true) {
        m_autoRefresh = enable;
        m_onlyChangedItems = only_changed_items;
        if (m_autoRefresh && m_timerId == -1)
            m_timerId = startTimer(refreshInterval);
        else if (!m_autoRefresh && m_timerId != -1){
            killTimer(m_timerId);
            m_timerId = -1;
        }
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
            updateItemsIndex();

            endRemoveRows();
            return true;
        }
        return false;
    }

    void updateItemsIndex() {

        // Redefining row Numbers
        for (int i = 0; i < items.count(); ++i) {
            items[i]->setRow(i);
        }
    }

protected:

    void timerEvent(QTimerEvent *event) override {
        if (event->timerId() != m_timerId)
            return;
        if  (m_onlyChangedItems){
            updateChangedItems();
        }
        else{
            updateAll();
        }
    }
public:
    int refreshInterval = 100; // ms, UI update cycle

private:
    QHash<QString, int> roleNameToId;
    QHash<int, QByteArray> roleIdToName;
    QVector<ItemType*> items;
    QHash <quint32 , quint32>mapItemIdToRow;// trgId--->index item
    QHash<quint32 , ItemType*>mapItemIdToObject;//itemId , object

    int m_timerId = -1;
    bool m_autoRefresh = false;
    bool m_onlyChangedItems = true;
    int m_selectedIndex = -1; // Latest index selected

};

#endif // QITEMTABLEMODEL_H
