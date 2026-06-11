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
        std::cerr << "Warning! Frontend not found on path:" << html_path.string() << std::endl;
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

    CROW_ROUTE(app, "/host-media").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        auto path_param = req.url_params.get("path");
        if (!path_param) return crow::response(400, "Missing path parameter");

        std::string file_path = path_param;

        if (!std::filesystem::exists(file_path)) {
            return crow::response(404, "Image file not found inside container");
        }

        std::ifstream in(file_path, std::ios::in | std::ios::binary);
        if (!in) return crow::response(500, "Cannot open image file");

        std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

        crow::response res(contents);

        auto has_extension = [](const std::string& str, const std::string& ext) {
            return str.size() >= ext.size() && str.rfind(ext) == (str.size() - ext.size());
        };

        if (has_extension(file_path, ".png")) {
            res.set_header("Content-Type", "image/png");
        } else if (has_extension(file_path, ".jpg") || has_extension(file_path, ".jpeg")) {
            res.set_header("Content-Type", "image/jpeg");
        } else if (has_extension(file_path, ".webp")) {
            res.set_header("Content-Type", "image/webp");
        } else {
            res.set_header("Content-Type", "application/octet-stream");
        }

        return res;
    });

    CROW_ROUTE(app, "/images/<string>")
    ([](std::string filename) {
        crow::response res;
        res.set_static_file_info("frontend/images/" + filename);
        return res;
    });

    CROW_ROUTE(app, "/resource/select-image").methods(crow::HTTPMethod::POST)([](){
        const std::string target_file = "/app/selected_path.txt";

        {
            std::ofstream clear_file(target_file, std::ios::trunc);
            clear_file << "WAITING\n";
        }

        std::ofstream pipe("/app/pipe.txt");
        if (pipe.is_open()) {
            pipe << "SELECT_IMAGE\n";
            pipe.close();
            std::cout << "[Backend] Sent SELECT_IMAGE to pipe directly!" << std::endl;
        } else {
            std::cout << "[Backend] window: Failed to open pipe.txt" << std::endl;
            return crow::response(500, "Cannot open FIFO pipe");
        }

        int timeout = 0;
        std::string path = "WAITING";

        while (timeout < 60) {
            std::filesystem::directory_iterator end_itr;
            for (std::filesystem::directory_iterator itr("/app"); itr != end_itr; ++itr) {
                if (itr->path().string() == target_file) break;
            }

            if (std::filesystem::exists(target_file)) {
                std::ifstream file(target_file);
                if (file.is_open()) {
                    std::getline(file, path);
                }
            }

            if (path != "WAITING" && !path.empty()) {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            timeout++;
        }

        std::cout << "[Backend] Got the path from the file:" << path << std::endl;

        if (path == "CANCELLED" || path == "WAITING" || path.empty()) {
            return crow::response(400, "Selection cancelled or timeout");
        }

        crow::json::wvalue dto;
        dto["path"] = path;
        return crow::response(dto);
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
