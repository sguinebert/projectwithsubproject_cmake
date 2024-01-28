#include "JsBridge.h"

EM_JS(void, callJSFunction, (const char* str), {
    // Convert the C string to a JavaScript string
    console.log('C++ string in JS:', UTF8ToString(str));
    Wt.emit(screenElement, 'message', UTF8ToString(str));
});

void JsBridge::executeJavaScript(const QString &jsCode) {

    callJSFunction(jsCode.toStdString().c_str());

    // EM_ASM({
    //     var screenElement = document.getElementById("screen");
    //     Wt.emit(screenElement, 'message', 'foo');
    // });
}
