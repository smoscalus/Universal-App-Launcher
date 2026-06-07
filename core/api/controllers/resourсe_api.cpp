#include <fstream>
#include <filesystem>

#include "resource_api.h"
#include "../../application/dto/Resource.h"
#include "../infrastructure/install/include/Crow/crow_all.h"

void ResourceController::setup_routes() { 

    CROW_ROUTE(_app, "/resource").methods(crow::HTTPMethod::POST) 
    ([this](const crow::request& req) { 
        auto data = crow::json::load(req.body); 
        if (!data) return crow::response(400, "Invalid JSON"); 

        DTO::CreateResourceRequest createReq;
        createReq.name = data["name"].s();
        createReq.description = data.has("description") ? std::string(data["description"].s()) : "";
        createReq.path = data["path"].s();
        createReq.avatar_url = data.has("avatar_url") ? std::string(data["avatar_url"].s()) : ""; // Читаем при POST
        createReq.user_id = data.has("user_id") ? data["user_id"].u() : 0;
        createReq.category_id = data.has("category_id") ? data["category_id"].u() : 0;

        int id = _service.createResource(createReq); 
        if (id < 0) return crow::response(500, "DB Error"); 

        DTO::CreateResourceResponse resDto{
            static_cast<uint64_t>(id),
            true,
            "Resource created successfully"
        };
        return crow::response(201, resDto.to_json()); 
    });

    CROW_ROUTE(_app, "/resource/<int>")
    ([this](int id) {
        auto dto = _service.getResourceById(static_cast<uint64_t>(id));
        
        if (dto.id == 0) {
            return crow::response(404, "Not Found");
        }
        
        return crow::response(dto.to_json());
    });


    CROW_ROUTE(_app, "/user/<int>/resources")
    ([this](int userId) {
        auto resources = _service.getResourcesByUserId(static_cast<uint64_t>(userId));
        
        std::vector<crow::json::wvalue> list;
        for (const auto& r : resources) {
            list.push_back(r.to_json());
        }
        return crow::json::wvalue(list);
    });

    CROW_ROUTE(_app, "/resource/<int>").methods(crow::HTTPMethod::PUT)
    ([this](const crow::request& req, int id) {
        auto data = crow::json::load(req.body);
        if (!data) return crow::response(400, "Invalid JSON");

        DTO::CreateResourceRequest updateReq;
        updateReq.name = data["name"].s();
        updateReq.description = data.has("description") ? std::string(data["description"].s()) : "";
        updateReq.path = data["path"].s();
        updateReq.user_id = data.has("user_id") ? data["user_id"].u() : 0;
        updateReq.category_id = data.has("category_id") ? data["category_id"].u() : 0;

        if (data.has("avatar_url")) {
            std::string base64data = data["avatar_url"].s();
            if (!base64data.empty() && base64data.find("base64,") != std::string::npos) {

                std::string encoded = base64data.substr(base64data.find("base64,") + 7);
                std::string decoded = crow::utility::base64decode(encoded);

                std::filesystem::create_directories("frontend/images");

                std::string file_path = "frontend/images/resource_" + std::to_string(id) + ".png";
                std::ofstream out(file_path, std::ios::binary);
                if (out) {
                    out.write(decoded.data(), decoded.size());
                    out.close();
                }

                updateReq.avatar_url = "/images/resource_" + std::to_string(id) + ".png";
            } else {
                updateReq.avatar_url = base64data;
            }
        }

        try {
            DTO::ResourceDto updateRes = _service.updateResource(id, updateReq); 
            return crow::response(200, updateRes.to_json());
        }
        catch (const std::exception& e) {
            std::cerr << "Resource Error: " << e.what() << std::endl;
            return crow::response(444, "Database error or Resource not found"); 
        }
    });

    CROW_ROUTE(_app, "/resource/launch/<int>").methods(crow::HTTPMethod::POST) 
    ([this](int id) { 
        if (_service.launchResource(static_cast<uint64_t>(id))) {
            return crow::response(200, "Launched"); 
        }
        return crow::response(404, "Not Found or Launch Failed"); 
    }); 

    CROW_ROUTE(_app, "/resource/<int>").methods(crow::HTTPMethod::DELETE) 
    ([this](int id) { 
        _service.deleteResource(static_cast<uint64_t>(id)); 
        return crow::response(200); 
    }); 
}
