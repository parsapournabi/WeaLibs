#include "worker.h"
#include <QRandomGenerator>
#include <QThread>

Worker::Worker(QObject *parent)
    : QObject{parent}
{
    QThread *thread = new QThread();
    this->moveToThread(thread);
    QObject::connect(thread, &QThread::started, this, &Worker::timelaps);
    thread->start();

}

void Worker::timelaps() {
    // Example of data assigning
    while (true) {
        if (!loop)
            continue;
        if (targets->isEmpty())
            return;
        for (int i = 0; i < targets->countItem(); ++i) {
            if (!targets->validItemId(i)) {
                qDebug() << "Not contains: " << i;
                continue;

            }
            double max = 360.0;
            double maxr = 240.0;
            Target* target = targets->returnItemObject(i);
            target->setAzimuth(static_cast<float>(QRandomGenerator::global()->bounded(max)));
            target->setElevation(static_cast<float>(QRandomGenerator::global()->bounded(maxr)));
            target->setRangeCell(static_cast<float>(QRandomGenerator::global()->bounded(maxr)));
            target->setPower(static_cast<float>(QRandomGenerator::global()->bounded(maxr)));
        }

    }
}
