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


    CROW_ROUTE(_app, "/resources") 
    ([this]() { 
        auto allDto = _service.getAllResources(); 
        std::vector<crow::json::wvalue> list; 

        for (const auto& dto : allDto) {
            list.push_back(dto.to_json()); 
        } 

        return crow::response(crow::json::wvalue(list)); 
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