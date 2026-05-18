#include <iostream>
#include <filesystem>
#include "../infrastructure/install/include/Crow/crow_all.h"

#include "controllers/resource_api.h"
#include "controllers/user_api.h"
#include "controllers/category_api.h"
#include "controllers/tag_api.h"
#include "controllers/preset_api.h"

#include "../application/services/resource_service.h"
#include "../application/services/user_service.h"
#include "../application/services/category_service.h"
#include "../application/services/tag_service.h"
#include "../application/services/preset_service.h"

#include "../infrastructure/install/include/hellnah/Engine/Database.h" 

int main() {
    crow::App<crow::CORSHandler> app;
    namespace fs = std::filesystem;

    if (!fs::exists("hellnah")) {
        fs::create_directory("hellnah");
    }

    Engine::Database resourceDb("hellnah/resources.hellnot");
    Engine::Database categoryDb("hellnah/categorys.hellnot");
    Engine::Database userDb("hellnah/users.hellnot");
    Engine::Database tagDb("hellnah/tags.hellnot");
    Engine::Database resourceTagDb("hellnah/resource_tags.hellnot");
    Engine::Database presetDb("hellnah/presets.hellnot");
    Engine::Database presetResourceDb("hellnah/preset_resources.hellnot");

    ResourceService resourceService(resourceDb);
    UserService userService(userDb);
    CategoryService categoryService(categoryDb, userService);
    TagService tagService(tagDb, resourceTagDb);
    PresetService presetService(presetDb, presetResourceDb, resourceService);
    
    ResourceController resourceApi(app, resourceService);
    CategoryController categoryApi(app, categoryService);
    UserController userApi(app, userService);
    TagController tagApi(app, tagService);
    PresetController presetApi(app, presetService);

    resourceApi.setup_routes();
    userApi.setup_routes();
    categoryApi.setup_routes();
    tagApi.setup_routes();
    presetApi.setup_routes();

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

    app.port(18080).run();
}
