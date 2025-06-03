#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QLoggingCategory>
#include <qloggingcategory.h>
#include <QThread>
#include "TargetModel.h"
#include "worker.h"
#include "fps.h"

int main(int argc, char *argv[])
{
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif


        QGuiApplication app(argc, argv);

        qputenv("QT_ASSUME_STDERR_HAS_CONSOLE", "1");
//        qputenv("QSG_INFO", "1");


        TargetModel targetModel;
        Target *t = new Target(1.0, 2.0, 3.0, 4.0);
//        qDebug() << t->getProperties();
        Target *t1 = new Target(1.0, 2.0, 3.0, 4.0);
        Target *t2 = new Target(1.0, 2.0, 3.0, 4.0);

        targetModel.insertItem(t, 0);
        targetModel.insertItem(t2, 1);
        targetModel.insertItem(t1, 2);
        Worker* worker = new Worker();
        for (int i = 3; i < 4; i++) {
            Target *tar = new Target();
            targetModel.insertItem(tar, i);
            worker->targets.append(tar);
        }
        worker->loop = true;
        qDebug() << targetModel.roleNames();
        qmlRegisterType<Fps>("CustomItems", 1, 0, "Fps");

        QQmlApplicationEngine engine;
        const QUrl url(QStringLiteral("qrc:/main.qml"));
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
            &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
        engine.rootContext()->setContextProperty("targetModel", &targetModel);
//        engine.rootContext()->setContextProperty("fpsC", &fpsC);
        engine.load(url);
        return app.exec();


//    QQmlApplicationEngine engine;
//    QObject::connect(
//        &engine,
//        &QQmlApplicationEngine::objectCreationFailed,
//        &app,
//        []() { QCoreApplication::exit(-1); },
//        Qt::QueuedConnection);
//    engine.rootContext()->setContextProperty("targetModel", &targetModel);
//    engine.loadFromModule("Radar", "Main");

}
