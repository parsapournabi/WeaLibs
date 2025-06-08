#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include "models/examples/TargetModel.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);

    void timelaps();

    TargetModel *targets;

    QThread *thread;

    TargetModel *model;
    bool loop = false;

signals:

};

#endif // WORKER_H
