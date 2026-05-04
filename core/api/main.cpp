#include "../infrastructure/install/include/Crow/crow_all.h"
#include <iostream>

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/api/status")
    ([]() {
        crow::json::wvalue res;
        res["status"] = "online";
        res["engine"] = "Crow C++";
        res["database"] = "HELL NAH (ready)";
        
        return res;
    });

    std::cout << "🚀 Server started on http://localhost:18080" << std::endl;
    app.port(18080).multithreaded().run();
}