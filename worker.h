#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QTimer>
#include "TargetModel.h"
#include "Target.h"
class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);

    void timelaps();

    QTimer *timer;

    QVector<Target*> targets;

    QThread *thread;

    TargetModel *model;
    bool loop = false;

signals:

};

#endif // WORKER_H
