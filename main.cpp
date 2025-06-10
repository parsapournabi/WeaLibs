#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QLoggingCategory>
#include <qloggingcategory.h>
#include <QThread>

//#define USE_NETWORK_SOURCE
#ifdef USE_NETWORK_SOURCE
    #include <WeaNet/Manager.h>
#else
    #include "common/worker.h"
#endif
#include "common/fps.h"
#include "models/examples/SortFilterProxy.h"
//#include "models/QItemSortFilterProxyModel.h"


int main(int argc, char *argv[])
{
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif


    QGuiApplication app(argc, argv);


    qputenv("QT_ASSUME_STDERR_HAS_CONSOLE", "1");
//    qputenv("QSG_INFO", "1");
    qmlRegisterType<Fps>("CustomItems", 1, 0, "Fps");
    qmlRegisterType<SortFilterProxyModel>("CustomItems", 1, 0, "SortFilterProxyModel");

    TargetModel targetModel;
    // Custom Filtering class
    SortFilterProxyModel filter;
    filter.setSourceModel(&targetModel);
    filter.setFilterCaseSensitivity(Qt::CaseInsensitive);

#ifdef USE_NETWORK_SOURCE
    Target *tar = new Target();
    targetModel.insertItem(tar, 0);

    QThread threadRecv = QThread();
    Manager* receiver = new Manager();
    qDebug() << QThread::currentThreadId();
    receiver->moveToThread(&threadRecv);
//    QMetaObject::invokeMethod(receiver, "setConnectionType", Qt::QueuedConnection, Q_ARG(int, 2));
    receiver->setConnectionType(1); // Udp (0= TcpClient, 1 = TcpServer)
    receiver->setConnectionSetting(54321, 54321, "172.16.50.50"); // Change it to your Address & port
    receiver->onBind();
//    receiver->onConnect();

    QObject::connect(receiver, &Manager::readyReads, receiver, [&] (QSharedPointer<QList<QSharedPointer<LogDataType>>> logDatas,
                                                                   qreal az, qreal time) {
//            if (logDatas->size() > 1)
            qDebug() << "logDataSize is: " << logDatas->size() << az << time;
            for (int i = 0; i < logDatas->size(); ++i) {
                qDebug() << logDatas->at(i)->rawVec();
            }
            Target *target = targetModel.returnItemObject(0);
            target->setAzimuth(logDatas->constLast()->azimuth());
            target->setElevation(logDatas->constLast()->elevation());
            target->setRangeCell(logDatas->constLast()->range());
            target->setPower(logDatas->constLast()->power());
        }, Qt::DirectConnection);

//    QObject::connect(receiver, &Manager::readyRead, receiver, [&] (LogDataType *obj) {
//            Target *target = targetModel.returnItemObject(0);
//            target->setAzimuth(obj->azimuth());
//            target->setElevation(obj->elevation());
//            target->setRangeCell(obj->range());
//            target->setPower(obj->power());
//            obj->deleteLater();

//        }, Qt::AutoConnection);
    threadRecv.start();

#else
    Worker* worker = new Worker();
    for (int i = 0; i < 10000; i++) {
        Target *tar = new Target();
        targetModel.insertItem(tar, i);
    }
    worker->loop = true;
    worker->targets = &targetModel;
#endif

    qDebug() << targetModel.roleNames();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("tableModel", &targetModel);
    engine.rootContext()->setContextProperty("proxyModel", &filter); // If you want to use filtering else uncommnet above line
    engine.load(url);
    return app.exec();

}
