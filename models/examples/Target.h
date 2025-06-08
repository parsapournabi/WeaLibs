#ifndef TARGET_H
#define TARGET_H

#include <QObject>
#include <QDebug>
#include <QVariant>
#include "models/QItemBase.h"

class Target : public QItemBase {
    Q_OBJECT
    Q_PROPERTY(float azimuth READ azimuth WRITE setAzimuth CONSTANT)
    Q_PROPERTY(float elevation READ elevation WRITE setElevation CONSTANT)
    Q_PROPERTY(float rangeCell READ rangeCell WRITE setRangeCell CONSTANT)
    Q_PROPERTY(float power READ power WRITE setPower CONSTANT)
public:
    explicit Target(float az = 1.0, float elv = 1.0, float rc = 1.0 , float p = 1.0) :
        m_azimuth(az), m_elevation(elv), m_rangeCell(rc), m_power(p)
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
    void setAzimuth(float az) { m_azimuth = az;}

    float elevation() const{ return m_elevation; }
    void setElevation(float elv){ m_elevation = elv; }

    float rangeCell() const{ return m_rangeCell; }
    void setRangeCell(float range_cell){ m_rangeCell =  range_cell; }

    float power() const { return m_power; }
    void setPower(float power){ m_power = power; }


private:
    float m_azimuth = 0.0;
    float m_elevation = 0.0;
    float m_rangeCell = 0.0;
    float m_power = 0.0;

};

#endif // TARGET_H
