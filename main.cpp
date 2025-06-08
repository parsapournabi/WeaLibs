#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QLoggingCategory>
#include <qloggingcategory.h>
#include <QThread>

#include "common/worker.h"
#include "common/fps.h"
#include "models/examples/SortFilterProxy.h"

int main(int argc, char *argv[])
{
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif


        QGuiApplication app(argc, argv);

        qputenv("QT_ASSUME_STDERR_HAS_CONSOLE", "1");
//        qputenv("QSG_INFO", "1");
        qmlRegisterType<Fps>("CustomItems", 1, 0, "Fps");
        qmlRegisterType<SortFilterProxyModel>("CustomItems", 1, 0, "SortFilterProxyModel");

        TargetModel targetModel;
        // Custom Filtering class
        SortFilterProxyModel filter;
        filter.setSourceModel(&targetModel);
        filter.m_targetModel = &targetModel;
        filter.setFilterCaseSensitivity(Qt::CaseInsensitive);

        Worker* worker = new Worker();
        for (int i = 0; i < 10000; i++) {
            Target *tar = new Target();
            targetModel.insertItem(tar, i);
        }
        worker->loop = true;
        worker->targets = &targetModel;
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
