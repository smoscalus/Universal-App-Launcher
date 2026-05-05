#include <iostream>
#include <filesystem>
#include "../infrastructure/install/include/Crow/crow_all.h"

#include "controllers/resource_api.h"
#include "controllers/user_api.h"
#include "../application/services/resource_service.h"
#include "../application/services/user_service.h"
#include "../infrastructure/install/include/hellnah/Engine/Database.h" 

int main() {
    crow::App<crow::CORSHandler> app;
    namespace fs = std::filesystem;

    if (!fs::exists("hellnah")) {
        fs::create_directory("hellnah");
    }

    Engine::Database resourceDb("hellnah/resources.hellnot");
    Engine::Database userDb("hellnah/user.hellnot");

    ResourceService resourceService(resourceDb);
    UserService userService(userDb);

    ResourceController resourceApi(app, resourceService);
    UserController userApi(app, userService);

    resourceApi.setup_routes();
    userApi.setup_routes();


    CROW_ROUTE(app, "/api/status")([]() {
        crow::json::wvalue res;
        res["status"] = "online";
        res["engine"] = "Crow C++";
        res["database"] = "HELL NAH";
        return res;
    });

    app.get_middleware<crow::CORSHandler>().global().origin("*");

    std::cout << "========================================" << std::endl;
    std::cout << "  UNIVERSAL APP LAUNCHER BACKEND        " << std::endl;
    std::cout << "  Server: http://localhost:18080        " << std::endl;
    std::cout << "========================================" << std::endl;

    app.port(18080).multithreaded().run();
}
