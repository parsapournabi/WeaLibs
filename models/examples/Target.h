#ifndef TARGET_H
#define TARGET_H

#include <QObject>
#include <QDebug>
#include <QVariant>
#include <math.h>
#include "models/QItemBase.h"

class Target : public QItemBase {
    Q_OBJECT
    Q_PROPERTY(double azimuth READ azimuth WRITE setAzimuth CONSTANT)
    Q_PROPERTY(double elevation READ elevation WRITE setElevation CONSTANT)
    Q_PROPERTY(double rangeCell READ rangeCell WRITE setRangeCell CONSTANT)
    Q_PROPERTY(double power READ power WRITE setPower CONSTANT)
public:
    explicit Target(double az = 1.0, double elv = 1.0, double rc = 1.0 , double p = 1.0, QObject* parent = nullptr) :
        m_azimuth(az), m_elevation(elv), m_rangeCell(rc), m_power(p), QItemBase{parent}
    {
    }

    enum TargetRoles {
        AzimuthRole = Qt::UserRole + 1,
        ElevationRole,
        RangeCellRole,
        PowerRole,
        SelectedRole,
        RowRole,
        IdRole,
        TargetObjectRole,
        AllRole
    };

    static QHash<int, QByteArray> getRoles() {
        return {
            {AzimuthRole, "azimuth"},
            {ElevationRole, "elevation"},
            {RangeCellRole, "rangeCell"},
            {PowerRole, "power"},
            { SelectedRole, "selected" },
            { RowRole, "row" },
            { IdRole, "id" },
            {TargetObjectRole, "targetObject",},
            {AllRole, "all"}
        };
    }

    QVariant getValueByRole(int role) override {
        switch (role) {
        case AzimuthRole: return azimuth();
        case ElevationRole: return elevation();
        case RangeCellRole: return rangeCell();
        case PowerRole: return power();
        case SelectedRole: return selected();
        case RowRole: return row();
        case IdRole: return id();
        case TargetObjectRole: return QVariant::fromValue(this);

        default: return {};
        }

    }

    float azimuth() const { return m_azimuth; }
    void setAzimuth(double az) {
        if (m_azimuth == az)
            return;
        m_azimuth = az;
        setUpdated(true);
    }

    float elevation() const{ return m_elevation; }
    void setElevation(double elv){
        if (m_elevation == elv)
            return;
        m_elevation = elv;
        setUpdated(true);
    }

    float rangeCell() const{ return m_rangeCell; }
    void setRangeCell(double range_cell){
        if (m_rangeCell == range_cell)
            return;
        m_rangeCell =  std::isnan(range_cell) ? 100.0: range_cell;
        setUpdated(true);
    }

    float power() const { return m_power; }
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
