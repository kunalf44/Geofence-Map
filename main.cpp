#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngineQuick>
#include "serial.h"
#include "netcheck.h"
#include<geofence.h>
#include <gcp.h>

int main(int argc, char *argv[])
{
    QtWebEngineQuick::initialize();

    QGuiApplication app(argc, argv);
    qmlRegisterType<Net>("kunal",2,0,"Net");
    qmlRegisterType<Geofence>("kunal",3,0,"Geofence");
    SerialManager serial;
    // serial.startReading();
    Gcp gcp;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("serial", &serial);
    engine.rootContext()->setContextProperty("gcp", &gcp);
    const QUrl url(QStringLiteral("qrc:/Geofence_Map/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
