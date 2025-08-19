#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QObject>
#include <QRandomGenerator>
#include <QThread>
#include <QBasicTimer>
#include "GLChartview.h"
#include "GLSeriesStorage.h"

struct PointXYSelf : public PointXYBase {
    using PointXYBase::PointXYBase;
    QString type = "MyTypeID";
    QString name() const { return "PointXYSelf"; }
};
Q_DECLARE_METATYPE(PointXYSelf)
Q_DECLARE_METATYPE(QVector<PointXYSelf>)

class MainClass : public QObject {
    Q_OBJECT
public:
    MainClass()
    {
        timer.start(100, this);
    }
    void genPoints() {
        if (!canGen)
            return;
        canGen = false;

        series0 = QSharedPointer<GLSeriesStorage<PointXYBase>>::create();
        series0->append(PointXYBase{100.0f, 5.0f, 129, 199, 199, 50});

        series1 = QSharedPointer<GLSeriesStorage<PointXYSelf>>::create();
        QVector<PointXYSelf> vecps;
        vecps << PointXYSelf{100.0f, 10.001f, 128, 50, 1, 255};
        vecps << PointXYSelf{150.0f, 1.555f, 128, 1, 0, 255};
        double max = 360.0;
        for (int i = 0; i < 10000; ++i) {
            float x = static_cast<float>(QRandomGenerator::global()->bounded(max));
            float y = static_cast<float>(QRandomGenerator::global()->bounded(max));
            vecps << PointXYSelf{x, y, 1.0f, 128, 0, 255};
//            vecps << PointXYSelf{x, y};
        }
        series1->replace(vecps);
        series1->append(PointXYSelf{374.0f, 0.555f, 255, 0, 127, 32});
        series1->append(PointXYSelf{734.0f, 0.001f, 255, 0, 127, 32});


        GLChartView *instance = GLChartView::getInstance();
//        handle0 = instance->addSeries(series0);
//        handle1 = instance->addSeries(series1);
        QMetaObject::invokeMethod(instance, [=]() { instance->addSeries(series0);});
        QMetaObject::invokeMethod(instance, [=]() { instance->addSeries(series1);});

//        instance->genPoints();
        QMetaObject::invokeMethod(instance, "updateSeries");

        QObject::connect(series0.data(), &IGLSeriesView::selectionChanged, this, [](const QVariant &payload, int typeId) {
            qDebug() << "TypeId: " << typeId << qMetaTypeId<QVector<PointXYBase>>();
            const auto vec = payload.value<QVector<PointXYBase>>();
            qDebug() << "Vec size: " << vec.size() << vec.at(0).name() << vec.at(0).x() << vec.at(0).y() << vec.at(0).color.r << vec.at(0).color.g << vec.at(0).color.b << vec.at(0).color.a;
        });
        QObject::connect(series1.data(), &IGLSeriesView::selectionChanged, this, [](const QVariant &payload, int typeId) {
            qDebug() << "TypeId 1: " << typeId << qMetaTypeId<QVector<PointXYSelf>>();
            const auto vec = payload.value<QVector<PointXYSelf>>();
            qDebug() << "Vec size: " << vec.size() << vec.at(0).name() << vec.at(0).x() << vec.at(0).y() << vec.at(0).color.r << vec.at(0).color.g << vec.at(0).color.b << vec.at(0).color.a;
            for (int i = 0; i < vec.size(); ++i) {
                //                qDebug() <<vec.at(i).name() << vec.at(i).type << vec.at(i).x() << vec.at(i).y() << vec.at(i).color;
            }
        });
        //        QObject::connect(handle0, &GLSeriesHandle::selectionChanged, this, [](const QVariant &payload, int typeId) {
        //            qDebug() << "TypeId: " << typeId << qMetaTypeId<QVector<PointXYBase>>();
        //            const auto vec = payload.value<QVector<PointXYBase>>();
        //            qDebug() << "Vec size: " << vec.size() << vec.at(0).name() << vec.at(0).x() << vec.at(0).y() << vec.at(0).color.r << vec.at(0).color.g << vec.at(0).color.b << vec.at(0).color.a;
        //        });
        //        QObject::connect(handle1, &GLSeriesHandle::selectionChanged, this, [](const QVariant &payload, int typeId) {
        //            qDebug() << "TypeId 1: " << typeId << qMetaTypeId<QVector<PointXYSelf>>();
        //            const auto vec = payload.value<QVector<PointXYSelf>>();
        //            qDebug() << "Vec size: " << vec.size() << vec.at(0).name() << vec.at(0).x() << vec.at(0).y() << vec.at(0).color.r << vec.at(0).color.g << vec.at(0).color.b << vec.at(0).color.a;
        //            for (int i = 0; i < vec.size(); ++i) {
        //                //                qDebug() <<vec.at(i).name() << vec.at(i).type << vec.at(i).x() << vec.at(i).y() << vec.at(i).color;
        //            }
        //        });

    }

    Q_INVOKABLE void doStuff() { canGen = true; }

protected:
    void timerEvent(QTimerEvent *event) {
        if (timer.timerId() == event->timerId())
            genPoints();
    }

private:
    QSharedPointer<GLSeriesStorage<PointXYBase>> series0;
    QSharedPointer<GLSeriesStorage<PointXYSelf>> series1;
    GLSeriesHandle *handle0 = nullptr;
    GLSeriesHandle *handle1 = nullptr;
    QVector4D vec4;
    QBasicTimer timer;
    bool canGen = false;
};

#endif // MAINCLASS_H
