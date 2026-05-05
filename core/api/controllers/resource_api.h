
#include "../../application/services/ResourceService.h"

class ResourceController {
private:
    crow::App<crow::CORSHandler>& _app;
    ResourceService& _service;

public:
    ResourceController(crow::App<crow::CORSHandler>& app, ResourceService& service) 
        : _app(app), _service(service) {}

    void setup_routes() {

        CROW_ROUTE(_app, "/resource").methods(crow::HTTPMethod::POST)
        ([this](const crow::request& req) {
            auto data = crow::json::load(req.body);
            if (!data) return crow::response(400, "Invalid JSON");

            dm::Resource newRes{};
            std::strncpy(newRes.name, std::string(data["name"].s()).c_str(), sizeof(newRes.name) - 1);
            std::strncpy(newRes.path, std::string(data["path"].s()).c_str(), sizeof(newRes.path) - 1);
            newRes.category_id = data["categoryId"].u();

            bool success = _service.createResource(newRes);
            if (!success) return crow::response(500, "DB Error");

            crow::json::wvalue res;
            res["id"] = newRes.id;
            return crow::response(201, res);
        });

        CROW_ROUTE(_app, "/resources")
        ([this]() {
            auto all = _service.getAllResources();
            std::vector<crow::json::wvalue> list;
            for (const auto& r : all) {
                crow::json::wvalue item;
                item["id"] = r.id;
                item["name"] = r.name;
                item["path"] = r.path;
                list.push_back(std::move(item));
            }
            return crow::response(crow::json::wvalue(list));
        });

        //(Use Case #1)
        CROW_ROUTE(_app, "/resource/launch/<int>").methods(crow::HTTPMethod::POST)
        ([this](int id) {
            if (_service.launchResource(id)) return crow::response(200, "Launched");
            return crow::response(404, "Not Found or Launch Failed");
        });

        CROW_ROUTE(_app, "/resource/<int>").methods(crow::HTTPMethod::DELETE)
        ([this](int id) {
            _service.deleteResource(static_cast<uint64_t>(id));
            return crow::response(200);
        });
    }
};