#include <iostream>

#include <Wt/WServer.h>
#include <Wt/WConfig.h>
#include <Wt/WWebassembly.h>
#include <Wt/WText.h>
//#include <Wt/lsquic.

using namespace std;
using namespace Wt;

WLength perc100(100, LengthUnit::Percentage);


awaitable<void> message(std::string_view message)
{
    std::cout << "message from client " << std::endl;
    co_return;
}


awaitable<std::unique_ptr<Wt::WApplication>> createApplication(const WEnvironment& env)
{
    std::cerr << "createApplication" << std::endl;
    auto executor = co_await asio::this_coro::executor;

    auto app = std::make_unique<WApplication>(env);
    app->setHtmlClass("html body");
    app->setBodyClass("body");

    auto root = app->root();
    root->resize(perc100, perc100);
    //app->enableUpdates();

    app->instance()->setTitle("Testing Page");

    auto wasm = root->addWidget(std::make_unique<WWebassembly>("webassembly", "./webassembly/webassembly", "app/webassembly"));
    wasm->decorationStyle().setBackgroundColor(WColor(StandardColor::Cyan));
    wasm->resize(800, 600);

    //std::cerr << "--------> " << wasm << std::endl;

    wasm->link().connect([wasm] (auto view){
        //std::cerr << "--------> copy " << wasm << std::endl;

        std::cout << "------------------------------------message from client " << view << std::endl;
        wasm->test();

    });

    // asio::post(executor, [wasm]() {
    //     std::cerr << "--------> post copy " << wasm << std::endl;

    // });

    wasm->doJavaScript(R"""(
            var screenElement = document.getElementById("screen");
            Wt.emit(screenElement, 'message', 'bar');
    )""");

    co_return app;
}

int main(int argc, char **argv)
{

    try {
        //::Configuration conf("server_config.json");

        WServer server(argc, argv, WTHTTP_CONFIGURATION);

        server.addEntryPoint(EntryPointType::Application, std::bind(createApplication, std::placeholders::_1), "/app");

        std::string pp = "./webassembly/webassembly";
        auto wasmresource = new WMemoryResource("application/wasm",  pp + ".wasm");
        server.addResource(wasmresource, "webassembly.wasm");

        auto jsresource = new WMemoryResource("application/javascript", pp + ".js");
        server.addResource(jsresource, "webassembly.js");

        if (server.start()) {
            int sig = Wt::WServer::waitForShutdown();

            std::cerr << "Shutdown (signal = " << sig << ")" << std::endl;
            server.stop();
        }

    }
    catch (WServer::Exception& e) {
        std::cerr << "WServer::Exception: "<< e.what() << "\n";
        return 1;
    }
    catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << "\n";
        return 2;
    }
    return 0;
}
