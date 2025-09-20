#ifndef DATASOURCE_H
#define DATASOURCE_H

#include "WeaChart/series/GLSeriesStorage.h"

#include <QThread>
#include <QBasicTimer>

// Required if you want to pass a custom point struct on your series (GLSeriesStorage<T>).
struct PointSeries1 : public PointXYBase {
    using PointXYBase::PointXYBase;
    QString type = "typeSeries1";
    //    const char* type =  "MyTypeID";
    double az = 12.7;
    double elv = 178.77;
    int otherData = 3;
    PointSeries1(float x, float y,
                 float r, float g, float b, float a,
                 double az, double elv, int otherData,
                 const QString &type)
        :
        PointXYBase{x, y, r, g, b, a},
        az(az), elv(elv), otherData(otherData),
        type(type)

    {

    }
    QString name() const { return "PointSeries1"; }
};
Q_DECLARE_METATYPE(PointSeries1) // Also Required
Q_DECLARE_METATYPE(QVector<PointSeries1>) // Also Required

// Required if you want to pass a custom point struct on your series (GLSeriesStorage<T>).
struct PointSeries2 : public PointXYBase {
    using PointXYBase::PointXYBase;
    enum TypeEnum {
        TypeNumber1 = 0,
        TypeNumber2
    };
    TypeEnum typeEnum = TypeNumber2;

    PointSeries2(float x, float y,
                 float r, float g, float b, float a,
                 TypeEnum typeEnum)
        :
        PointXYBase{x, y, r, g, b, a},
        typeEnum(typeEnum)
    {

    }

    QString getTypeEnumString() const {
        switch (typeEnum) {
        case TypeNumber1:
            return "TypeNumber1";
        case TypeNumber2:
            return "TypeNumber2";
        default:
            return "Invalid typeEnum";
        }
    }
    QString name() const { return "PointSeries2"; }
};
Q_DECLARE_METATYPE(PointSeries2) // Also Required
Q_DECLARE_METATYPE(QVector<PointSeries2>) // Also Required

class DataSource : public QObject
{
    Q_OBJECT
public:
    DataSource(QObject *parent = nullptr);
    ~DataSource();

signals:

public slots:
    void generateData(); // This method is an example test method, isn't required on your application.
    void updateAllSeries();

protected:
    void makeConnenction();

    void timerEvent(QTimerEvent *event) override;

public:
    GLSeriesStorage<PointXYBase> *series0 = nullptr;
    GLSeriesStorage<PointSeries1> *series1 = nullptr;
    GLSeriesStorage<PointSeries2> *series2 = nullptr;


private:
    // Test Data only for generating
    QVector<QVector<PointXYBase>> m_seriesData0; // Row, column
    QVector<QVector<PointSeries1>> m_seriesData1; // Row, column
    QVector<QVector<PointSeries2>> m_seriesData2; // Row, column
    int m_index = 0;
    bool m_generated = false;

    // for updating data
    QBasicTimer timer;
    QThread thread;

};

#endif // DATASOURCE_H
