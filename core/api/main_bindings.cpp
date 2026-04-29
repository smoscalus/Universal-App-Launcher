#include <emscripten/bind.h>
#include <iostream>

using namespace emscripten;

void saveResource(int id, std::string name, std::string path) {
    std::cout << "CORE LOG: Saving resource [" << name << "] with path [" << path << "]" << std::endl;
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("saveResource", &saveResource);
}
