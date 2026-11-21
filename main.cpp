#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "stockapiclient.h"
#include "stockmodel.h"
#include <QQmlContext>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    StockModel stockModel;

    StockApiClient stockClient;
    stockClient.setStockModel(&stockModel);

    stockClient.fetchStockData("MOWI.OL");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(
        "stockModel", &stockModel);
    engine.rootContext()->setContextProperty(
        "stockApiClient", &stockClient);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("NordicStockDashboard", "Main");

    return QGuiApplication::exec();
}
