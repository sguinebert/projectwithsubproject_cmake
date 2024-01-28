#ifndef JSBRIDGE_H
#define JSBRIDGE_H

#if __has_include(<emscripten/emscripten.h>)
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#endif

#include <QObject>
#include <iostream>
//using namespace emscripten;


/*pass through javascript (expose the module to security concern)
 * vs create a direct webchannel (direct link more secure but create a second websocket to the server)
*/
class JsBridge : public QObject
{
    Q_OBJECT
public:
    explicit JsBridge(QObject *parent = nullptr) : QObject(parent)
    {}

public:
    Q_INVOKABLE void executeJavaScript(const QString& jsCode);

    void fromServer(std::string& message)
    {
        std::cout << "test js call : " << message << std::endl;
    }

};



#endif // JSBRIDGE_H
