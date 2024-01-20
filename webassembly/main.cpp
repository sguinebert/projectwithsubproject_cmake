#include <emscripten.h>
#include <iostream>

EM_JS(void, call_js_function, (), {
    // JavaScript code here
    // For example, calling a Wt function
    Wt.function();
});

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    return 0;
}
