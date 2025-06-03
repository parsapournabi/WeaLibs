#ifndef TARGETMODEL_H
#define TARGETMODEL_H

//#include <QAbstractTableModel>
#include "QItemListModel.h"
#include "Target.h"

class TargetModel: public QItemListModel<Target> {
    Q_OBJECT
public:
};

//class TargetModel : public QAbstractListModel {
////    Q_OBJECT

//public:
//    enum TargetRoles {
//        AzimuthRole = Qt::UserRole + 1,
//        ElevationRole,
//        RangeCellRole,
//        PowerRole,
//        SelectedRole,
//        RowRole,
//        TargetObjectRole,
//    };

//    explicit TargetModel(QObject *parent = nullptr);

//    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
//    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

//    QHash<int, QByteArray> roleNames() const override;

//    void addTarget(Target *target);
//    Target* getTarget(int row) const;

//    Q_INVOKABLE void addTargetFromQml(float az, float el, float range, float power);
//    Q_INVOKABLE void removeLast();
//    Q_INVOKABLE void selectTarget(int index);
//private:
//    QList<Target*> m_targets;
//    int lastRow = 0;
//};

#endif // TARGETMODEL_H
