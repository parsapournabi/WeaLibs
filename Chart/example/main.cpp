#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSurfaceFormat>
#include <QQmlContext>
#include <QDebug>
#include <QThread>

#include "fps.h"
#include "GLChartview.h"
#include "datasource.h"

int main(int argc, char *argv[])
{

    QSurfaceFormat fmt;
    fmt.setMajorVersion(4);
    fmt.setMinorVersion(6);
    fmt.setDepthBufferSize(24);
    fmt.setSwapInterval(1);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);

    QSurfaceFormat::setDefaultFormat(fmt);
    QSurfaceFormat curFmt = QSurfaceFormat::defaultFormat();

    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif

    QGuiApplication app(argc, argv);

    qputenv("QT_ASSUME_STDERR_HAS_CONSOLE", "1");
    qputenv("QSG_RENDER_LOOP", "basic");
//    qputenv("QSG_INFO", "1");
    qInfo() << "Surface Format: "
            << curFmt.renderableType()
            << curFmt.majorVersion()
            << curFmt.minorVersion()
            << curFmt.profile()
            << curFmt.swapInterval();

    qmlRegisterType<Fps>("CustomItems", 1, 0, "Fps");


    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    GLChartView::registerMetaTypes();
    DataSource dataSource;

    engine.rootContext()->setContextProperty("series0Ptr", dataSource.series0);
    engine.rootContext()->setContextProperty("series1Ptr", dataSource.series1);
    engine.rootContext()->setContextProperty("series2Ptr", dataSource.series2);

    engine.load(url);
    return app.exec();

}
