#include "worker.h"
#include <QTimer>
#include <QRandomGenerator>
#include <QThread>

Worker::Worker(QObject *parent)
    : QObject{parent}
{
//    timer = new QTimer();
//    QObject::connect(timer, &QTimer::timeout, this, &Worker::timelaps);
    QThread *thread = new QThread();
    this->moveToThread(thread);

    QObject::connect(thread, &QThread::started, this, &Worker::timelaps);

    thread->start();

}

void Worker::timelaps() {
    while (true) {
        if (!loop)
            continue;
        if (targets.isEmpty())
            return;
        for (auto &target : targets) {
            double max = 360.0;
            double maxr = 240.0;
            target->setAzimuth(static_cast<float>(QRandomGenerator::global()->bounded(max)));
            target->setElevation(static_cast<float>(QRandomGenerator::global()->bounded(maxr)));
            target->setRangeCell(static_cast<float>(QRandomGenerator::global()->bounded(maxr)));
            target->setPower(static_cast<float>(QRandomGenerator::global()->bounded(maxr)));
        }
//        emit model->dataChanged(model->index(0), model->index(model->rowCount() - 1));
//        QThread::msleep(1);

    }
}
