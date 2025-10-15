#include "datasource.h"

#include <QElapsedTimer>
#include <QRandomGenerator>
#include <qmath.h>

DataSource::DataSource(QObject *parent)
    : QObject{parent},
    series0(new GLSeriesStorage<PointXYBase>()),
    series1(new GLSeriesStorage<PointSeries1>()),
    series2(new GLSeriesStorage<PointSeries2>())
{
    // Thread
    this->moveToThread(&thread);
    connect(&thread, &QThread::started, this, [=] () { timer.start(16, this); });
    connect(&thread, &QThread::started, this, [=] () {
//        QElapsedTimer ep;
//        ep.start();
        generateData();
//        qDebug() << "Elapsed Timer at generateData(): "
//                 << ep.elapsed()
//                 << QThread::currentThreadId();
    });
    connect(&thread, &QThread::finished, this, [=] () { timer.stop(); });
    connect(this, &DataSource::destroyed, &thread, [=]() { timer.stop(); thread.quit(); thread.wait(); });


    // Connections
    makeConnenction();

    // Sample Triangle for series0 on initialize
    series0->append(PointXYBase{50.0f, 50.0f, 255, 0, 0, 1.0});
    series0->append(PointXYBase{40.0f, 25.0f, 0, 255, 0, 1.0});
    series0->append(PointXYBase{60.0f, 25.0f, 0, 0, 255, 1.0});
    series0->append(PointXYBase{50.0f, 50.0f, 255, 0, 0, 1.0});

    // Sample Triangle for series1 on initialize
    series1->append(PointSeries1{100.0f, 50.0f, 255, 0, 0, 1.0});
    series1->append(PointSeries1{90.0f, 25.0f, 0, 255, 0, 1.0});
    series1->append(PointSeries1{110.0f, 25.0f, 0, 0, 255, 1.0});
    series1->append(PointSeries1{100, 50.0f, 255, 0, 0, 1.0});

    // Sample Triangle for series2 on initialize
    series2->append(PointSeries2{50.0f, 100.0f, 255, 0, 0, 1.0});
    series2->append(PointSeries2{40.0f, 75.0f, 0, 255, 0, 1.0});
    series2->append(PointSeries2{60.0f, 75.0f, 0, 0, 255, 1.0});
    series2->append(PointSeries2{50.0f, 100.0f, 255, 0, 0, 1.0});

    thread.start();
}

DataSource::~DataSource()
{
    thread.quit();
    thread.wait();
}

void DataSource::generateData()
{
    if (m_generated) return;
    constexpr int ROW = 5;
    constexpr int SERIES0_TOTAL_COUNT = 200'000;
    constexpr int SERIES1_TOTAL_COUNT = 200'000;
    constexpr int SERIES2_TOTAL_COUNT = 200'000;
    constexpr int SERIES0_Y_OFFSET = 0;
    constexpr int SERIES1_Y_OFFSET = 10;
    constexpr int SERIES2_Y_OFFSET = 20;
    // Series 0 Data
    m_seriesData0.reserve(ROW);
    for (int i(0); i < ROW; ++i) {
        QVector<PointXYBase> points;
        points.reserve(SERIES0_TOTAL_COUNT);
        for (int j(0); j < SERIES0_TOTAL_COUNT; ++j) {
            float x(0.0f), y(SERIES0_Y_OFFSET);
            x = j;
            y += qSin(M_PI / 50 * j) + 0.5 + QRandomGenerator::global()->generateDouble();
            float r = static_cast<float>(QRandomGenerator::global()->bounded(1.0f));
            float g = static_cast<float>(QRandomGenerator::global()->bounded(1.0f));
            float b = static_cast<float>(QRandomGenerator::global()->bounded(1.0f));
            float a = static_cast<float>(QRandomGenerator::global()->bounded(1.0f));
            points.append({x, y, r, g, b, a});
        }
        m_seriesData0.append(points);
    }
    // Series 1 Data
    m_seriesData1.reserve(ROW);
    for (int i(0); i < ROW; ++i) {
        QVector<PointSeries1> points;
        points.reserve(SERIES1_TOTAL_COUNT);
        for (int j(0); j < SERIES1_TOTAL_COUNT; ++j) {
            float x(0.0f), y(SERIES1_Y_OFFSET);
            x = j;
            y += qSin(M_PI / 50 * j) + 0.5 + QRandomGenerator::global()->generateDouble();
            float r = static_cast<float>(QRandomGenerator::global()->bounded(1.0f));
            float g = static_cast<float>(QRandomGenerator::global()->bounded(1.0f));
            float b = static_cast<float>(QRandomGenerator::global()->bounded(1.0f));
            float a = static_cast<float>(QRandomGenerator::global()->bounded(1.0f));
            double az = i * j;
            double elv = j * y * 0.01;
            int otherData = i;

            points.append({x, y, r, g, b, a, az, elv, otherData, "TypeSeries1"});
//            points.append({x, y, r, g, b, a});
        }
        m_seriesData1.append(points);
    }
    // Series 2 Data
    m_seriesData2.reserve(ROW);
    for (int i(0); i < ROW; ++i) {
        QVector<PointSeries2> points;
        points.reserve(SERIES2_TOTAL_COUNT);
        for (int j(0); j < SERIES2_TOTAL_COUNT; ++j) {
            float x(0.0f), y(SERIES2_Y_OFFSET);
            x = j;
            y += qSin(M_PI / 50 * j) + 0.5 + QRandomGenerator::global()->generateDouble();
            float r = static_cast<float>(QRandomGenerator::global()->bounded(1.0f));
            float g = static_cast<float>(QRandomGenerator::global()->bounded(1.0f));
            float b = static_cast<float>(QRandomGenerator::global()->bounded(1.0f));
            float a = static_cast<float>(QRandomGenerator::global()->bounded(1.0f));
            PointSeries2::TypeEnum typeEnum = j % 2 == 0 ? PointSeries2::TypeNumber1 : PointSeries2::TypeNumber2;

            points.append({x, y, r, g, b, a, typeEnum});
//            points.append({x,y, r, g, b, a});
        }
        m_seriesData2.append(points);
    }
    m_generated = true;
}

void DataSource::updateAllSeries()
{
    // Assigning Data
    series0->replace(m_seriesData0.at(m_index));
    series1->replace(m_seriesData1.at(m_index));
    series2->replace(m_seriesData2.at(m_index));

    // Request to update all series on chartView
    /** NOTE: To increase efficiency don't requestUpdateSeriesData for each series.
                By calling 'requestUpdateSeriesData()', all series going ot update. **/
    series0->requestUpdateSeriesData();

    // Updating index
    m_index++;
    if (m_index >= m_seriesData0.size() ||
        m_index >= m_seriesData1.size() ||
        m_index >= m_seriesData2.size())
        m_index = 0;
}

void DataSource::makeConnenction()
{
    // Series0 Select Signal
    connect(series0, &GLAbstractSeries::selected, this, [](const QVariant &payload, int typeId) {
        const QVector<PointXYBase> selectedPoint = payload.value<QVector<PointXYBase>>();
        // Your selecting process...
        for (auto &point : selectedPoint) {
            qDebug() << "Series0 point: " << point.name() << point.x() << point.y() << point.rgba();
        }
    });
    // Series1 Select Signal
    connect(series1, &GLAbstractSeries::selected, this, [](const QVariant &payload, int typeId) {
        const QVector<PointSeries1> selectedPoint = payload.value<QVector<PointSeries1>>();
        // Your selecting process...
        for (auto &point : selectedPoint) {
            qDebug() << "Series1 point: " << point.name() << point.x() << point.y() << point.rgba()
                     << point.type << point.az << point.otherData; // Non-PointXYBase fields
        }
    });
    // Series2 Select Signal
    connect(series2, &GLAbstractSeries::selected, this, [](const QVariant &payload, int typeId) {
        const QVector<PointSeries2> selectedPoint = payload.value<QVector<PointSeries2>>();
        // Your selecting process...
        for (const PointSeries2 &point : selectedPoint) {
            qDebug() << "Series2 point: " << point.name() << point.x() << point.y() << point.rgba()
                     << point.getTypeEnumString(); // Non-PointXYBase fields
        }
    });
}

void DataSource::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer.timerId()) {
//        QElapsedTimer ep;
//        ep.start();
        updateAllSeries();
//        qDebug() << "Elapsed Timer on updateAllSeries(): " << ep.elapsed();
    }
}
