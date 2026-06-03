#include <iostream>
#include <filesystem>
#include <thread>
#include <filesystem>

#include "../infrastructure/install/include/Crow/crow_all.h"
#include "../infrastructure/install/include/webview_sdk/webview.h"

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

#include "../infrastructure/persistence/db_context.h"

namespace fs = std::filesystem;

int main() {
    crow::App<crow::CORSHandler> app;
    namespace fs = std::filesystem;

    if (!fs::exists("hellnah")) {
        fs::create_directory("hellnah");
    }

    auto dbContext = std::make_shared<DbContext>("hellnah");

    ResourceService resourceService(dbContext);
    UserService userService(dbContext);
    CategoryService categoryService(dbContext, userService);
    TagService tagService(dbContext);
    PresetService presetService(dbContext, resourceService);

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

    fs::path current_run_dir = fs::current_path();
    fs::path project_root = current_run_dir.parent_path();
    fs::path frontend_dir = project_root / "frontend";
    fs::path html_path = frontend_dir / "index.html";

    if (!fs::exists(html_path)) {
        std::cerr << "Предупреждение! Фронтенд не найден по пути: " << html_path.string() << std::endl;
    }

    CROW_ROUTE(app, "/")([html_path]() {
        crow::response res;
        std::ifstream file(html_path);
        if (file.is_open()) {
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            res.set_header("Content-Type", "text/html; charset=utf-8");
            res.body = content;
        } else {
            res.code = 404;
            res.body = "<h1>404: index.html not found</h1>";
        }
        return res;
    });

    CROW_ROUTE(app, "/<path>")([frontend_dir](std::string filepath) {
        crow::response res;
        fs::path full_path = frontend_dir / filepath;
        
        std::ifstream file(full_path, std::ios::binary);
        if (file.is_open()) {
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            res.body = content;
            res.code = 200;

            if (full_path.extension() == ".css") {
                res.set_header("Content-Type", "text/css; charset=utf-8");
            } else if (full_path.extension() == ".js") {
                res.set_header("Content-Type", "application/javascript; charset=utf-8");
            } else if (full_path.extension() == ".png") {
                res.set_header("Content-Type", "image/png");
            } else if (full_path.extension() == ".jpg" || full_path.extension() == ".jpeg") {
                res.set_header("Content-Type", "image/jpeg");
            } else if (full_path.extension() == ".svg") {
                res.set_header("Content-Type", "image/svg+xml");
            }
        } else {
            res.code = 404;
            res.body = "File not found: " + filepath;
        }
        return res;
    });

    app.get_middleware<crow::CORSHandler>().global().origin("*");

    std::cout << "========================================" << std::endl;
    std::cout << "  UNIVERSAL APP LAUNCHER BACKEND        " << std::endl;
    std::cout << "  Server: http://localhost:18080        " << std::endl;
    std::cout << "========================================" << std::endl;

    std::thread server_thread([&app]() {
        app.port(18080).run();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    try {
        webview::webview w(true, nullptr);
        w.set_title("Universal App Launcher");
        w.set_size(1024, 768, WEBVIEW_HINT_NONE);
        
        w.navigate("http://localhost:18080/"); 
        w.run();
    } 
    catch (const std::exception& e) {
        std::cerr << "Webview error: " << e.what() << std::endl;
    }

    std::cout << "Window closed. Stopping server..." << std::endl;
    app.stop();
    
    if (server_thread.joinable()) {
        server_thread.join();
    }

    std::cout << "The application has completed successfully." << std::endl;
    return 0;
}
