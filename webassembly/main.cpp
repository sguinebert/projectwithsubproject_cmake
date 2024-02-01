#include <iostream>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "JsBridge.h"


JsBridge* getJsBridgeInstance() {
    static JsBridge instance;
    return &instance;
}

void fromserver(std::string message) {
    auto instance = getJsBridgeInstance();
    instance->fromServer(message);
}

EMSCRIPTEN_BINDINGS(module) {
    // emscripten::class_<JsBridge>("JsBridge")
    //     .constructor<>()
    //     .function("myMethod", &JsBridge::myMethod);

    emscripten::function("update", &fromserver);
}


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    std::cout << "hello qt for webassembly dsfvdfvfdv" << std::endl;

    QQmlApplicationEngine engine;

    auto jsbridge = getJsBridgeInstance();
    engine.rootContext()->setContextProperty("jsbridge", jsbridge);


    const QUrl url(QStringLiteral("qrc:/main.qml")); // Adjust the path to your main QML file
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);
    //std::cout <<"d" <<std::endl;
    return app.exec();
}
