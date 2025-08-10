#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QLoggingCategory>
#include <qloggingcategory.h>
#include <QThread>

#include "fps.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qputenv("QT_ASSUME_STDERR_HAS_CONSOLE", "1");

}
