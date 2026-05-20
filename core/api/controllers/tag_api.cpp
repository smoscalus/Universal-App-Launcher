#include "tag_api.h"
#include "../../application/dto/Tag.h"

void TagController::setup_routes() {


    CROW_ROUTE(_app, "/tag").methods(crow::HTTPMethod::POST)
    ([this](const crow::request& req) {
        auto data = crow::json::load(req.body);
        if (!data) return crow::response(400, "Invalid JSON");

        DTO::CreateTagRequest createReq;
        createReq.name = data["name"].s();
        createReq.description = data.has("description") ? std::string(data["description"].s()) : "";
        createReq.user_id = data.has("user_id") ? data["user_id"].u() : 0;

        int id = _service.createTag(createReq);
        if (id < 0) return crow::response(500, "DB Error");

        DTO::CreateTagResponse resDto{
            static_cast<uint64_t>(id),
            true,
            "Tag created successfully"
        };
        return crow::response(201, resDto.to_json());
    });

    CROW_ROUTE(_app, "/user/<int>/tags")
    ([this](int userId) {
        auto tags = _service.getTagsByUserId(static_cast<uint64_t>(userId));
        
        std::vector<crow::json::wvalue> list;
        for (const auto& t : tags) {
            list.push_back(t.to_json());
        }
        return crow::json::wvalue(list);
    });

    CROW_ROUTE(_app, "/tag/<int>").methods(crow::HTTPMethod::PUT)
    ([this]( const crow::request& req, int id) {
        auto data = crow::json::load(req.body);
        if (!data) return crow::response(400, "Invalid JSON");

        DTO::CreateTagRequest updateReq;
        updateReq.name = data["name"].s();
        updateReq.description = data.has("description") ? std::string(data["description"].s()) : "";
        updateReq.user_id = data.has("user_id") ? data["user_id"].u() : 0;

        try {
            DTO::TagDto updateRes = _service.updateTag(id, updateReq); 
            return crow::response(200, updateRes.to_json());
        }
        catch (const std::exception& e) {
            std::cerr << "Tag Error: " << e.what() << std::endl;
            return crow::response(444, "Database error or Tag not found"); 
        }
    });

    CROW_ROUTE(_app, "/tag/<int>").methods(crow::HTTPMethod::DELETE)
    ([this](int id) {
        _service.deleteTag(static_cast<uint64_t>(id));
        return crow::response(200);
    });

    CROW_ROUTE(_app, "/resources/<int>/tags/<int>").methods(crow::HTTPMethod::POST)
    ([this](int resId, int tagId) {
        _service.linkTagToResource(static_cast<uint64_t>(resId), static_cast<uint64_t>(tagId));
        return crow::response(200, "Linked");
    });

    CROW_ROUTE(_app, "/resources/<int>/tags/<int>").methods(crow::HTTPMethod::DELETE)
    ([this](int resId, int tagId) {
        _service.unlinkTagFromResource(static_cast<uint64_t>(resId), static_cast<uint64_t>(tagId));
        return crow::response(200, "Unlinked");
    });

    CROW_ROUTE(_app, "/resource/<int>/tags")
    ([this](int resId) {
        auto tagIds = _service.getTagIdsByResource(static_cast<uint64_t>(resId));
        
        std::vector<crow::json::wvalue> list;
        for (uint64_t id : tagIds) {
            list.push_back(crow::json::wvalue(id));
        }
        return crow::json::wvalue(list);
    });
}
