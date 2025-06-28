#ifndef TARGET_H
#define TARGET_H

#include <QObject>
#include <QDebug>
#include <QVariant>
#include <math.h>
#include "models/QItemBase.h"

class Target : public QItemBase {
    Q_OBJECT
//    Q_PROPERTY(double azimuth READ azimuth WRITE setAzimuth CONSTANT)
//    Q_PROPERTY(double elevation READ elevation WRITE setElevation CONSTANT)
//    Q_PROPERTY(double rangeCell READ rangeCell WRITE setRangeCell CONSTANT)
//    Q_PROPERTY(double power READ power WRITE setPower CONSTANT)
public:
    explicit Target(double az = 1.0, double elv = 1.0, double rc = 1.0 , double p = 1.0, QObject* parent = nullptr) :
        m_azimuth(az), m_elevation(elv), m_rangeCell(rc), m_power(p), QItemBase{parent}
    {
    }

    enum TargetRoles {
        IdRole= Qt::UserRole + 1,
        RowRole,
        ItemSelectedRole,
        AzimuthRole,
        ElevationRole,
        RangeCellRole,
        PowerRole,
//        TargetObjectRole,
//        AllRole
    };
    Q_ENUM(TargetRoles)

    static QHash<int, QByteArray> getRoles() {
        return {
            {AzimuthRole, "azimuth"},
            {ElevationRole, "elevation"},
            {RangeCellRole, "rangeCell"},
            {PowerRole, "power"},
            { ItemSelectedRole, "itemSelected" },
            { RowRole, "row" },
            { IdRole, "id" },
//            {TargetObjectRole, "targetObject",},
//            {AllRole, "all"}
        };
    }

    QVariant getValueByRole(int role) override {
        switch (role) {
        case AzimuthRole: return azimuth();
        case ElevationRole: return elevation();
        case RangeCellRole: return rangeCell();
        case PowerRole: return power();
        case ItemSelectedRole: return itemSelected();
        case RowRole: return row();
        case IdRole: return id();
//        case TargetObjectRole: return QVariant::fromValue(this);

        default: return {};
        }

    }

    QString getTitleByRole(int role) const override {
        switch (role) {
        case AzimuthRole: return QString("Az");
        case ElevationRole: return QString("Elv");
        case RangeCellRole: return QString("Range Cell");
        case PowerRole: return QString("Power");
        case ItemSelectedRole: return QString("Selected");
        case RowRole: return QString("Row");
        case IdRole: return QString("Id");
        default: return QString();
        }
    }

    QVariantMap getHeaderDataByRole(int role) const override {
        switch (role) {
        case AzimuthRole: return QVariantMap({{"title", "Az"}, {"columnWidth", 100}, {"visible", true}});
        case ElevationRole: return QVariantMap({{"title", "Elv"}, {"columnWidth", 100}, {"visible", true}});;
        case RangeCellRole: return QVariantMap({{"title", "RangeCell"}, {"columnWidth", 100}, {"visible", true}});;
        case PowerRole: return QVariantMap({{"title", "Power"}, {"columnWidth", 100}, {"visible", true}});;
        case ItemSelectedRole: return QVariantMap({{"title", "Select"}, {"columnWidth", 100}, {"visible", true}});;
        case RowRole: return QVariantMap({{"title", "Row"}, {"columnWidth", 100}, {"visible", true}});;
        case IdRole: return QVariantMap({{"title", "ID"}, {"columnWidth", 100}, {"visible", true}});;
        default: return QVariantMap();
        }

    }


    double azimuth() const { return m_azimuth; }
    void setAzimuth(double az) {
        if (m_azimuth == az)
            return;
        m_azimuth = az;
        setUpdated(true);
    }

    double elevation() const{ return m_elevation; }
    void setElevation(double elv){
        if (m_elevation == elv)
            return;
        m_elevation = elv;
        setUpdated(true);
    }

    double rangeCell() const{ return m_rangeCell; }
    void setRangeCell(double range_cell){
        if (m_rangeCell == range_cell)
            return;
        m_rangeCell =  std::isnan(range_cell) ? 100.0: range_cell;
        setUpdated(true);
    }

    double power() const { return m_power; }
    void setPower(double power){
        if (m_power == power)
            return;
        m_power = power;
        setUpdated(true);
    }


private:
    double m_azimuth = 0.0;
    double m_elevation = 0.0;
    double m_rangeCell = 0.0;
    double m_power = 0.0;

};

#endif // TARGET_H
