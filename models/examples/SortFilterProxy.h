#ifndef SORTFILTERPROXY_H
#define SORTFILTERPROXY_H

#include <QSortFilterProxyModel>
#include <QObject>
#include <QDebug>
#include <QDate>

#include "TargetModel.h"

class SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString filterString READ filterString WRITE setFilterString NOTIFY filterStringChanged)
    Q_PROPERTY(TargetModel *targetModel READ targetModel CONSTANT) // Replace with your model class
public:
    explicit SortFilterProxyModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}

    TargetModel *targetModel() { return m_targetModel; } // Getter of modelClass

    QString filterString() const { return m_filterString; }

    Q_INVOKABLE void setFilterString(const QString &value) {
        if (m_filterString == value)
            return;
        m_filterString = value;
        setFilterRegularExpression(m_filterString);
        emit filterStringChanged();
    }

    // Model Functions
    Q_INVOKABLE void selectItem(int index) { m_targetModel->selectItem(index); }

    Q_INVOKABLE QVariantList getHeadersData(bool get_parent = true) { return m_targetModel->getHeadersData(get_parent); }

signals:
    void filterStringChanged();

protected:
 bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override {
     QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);

     // Custom Filtering queries
     return (m_targetModel->data(index0, Target::TargetRoles::IdRole).toString().contains(filterRegularExpression()));
 }
public:
    TargetModel *m_targetModel = nullptr;

private:
    QString m_filterString;

};

#endif // SORTFILTERPROXY_H
