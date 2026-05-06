#include "category_api.h"
#include "../../application/dto/Category.h"
#include "../infrastructure/install/include/Crow/crow_all.h"

void CategoryController::setup_routes() { 

    CROW_ROUTE(_app, "/category").methods(crow::HTTPMethod::POST)
    ([this](const crow::request& req) {
        auto data = crow::json::load(req.body);
        if (!data) return crow::response(400, "Invalid JSON"); 

        DTO::CreateCategoryRequest createReq;
        createReq.name = data["name"].s();
        createReq.description = data.has("description") ? std::string(data["description"].s()) : "";

        if (!data.has("user_id")) return crow::response(400, "Missing user_id");
        createReq.user_id = data["user_id"].u();

        int id = _service.createCategory(createReq); 

        if (id < 0) return crow::response(500, "Category Creation Failed"); 

        crow::json::wvalue res;
        res["id"] = static_cast<uint64_t>(id);
        res["success"] = true;

        return crow::response(201, res); 
    });

    CROW_ROUTE(_app, "/user/<int>/categories")
    ([this](int userId) {
        auto categories = _service.getCategoriesByUserId(static_cast<uint64_t>(userId));

        std::vector<crow::json::wvalue> list;
        for (const auto& c : categories) {
            list.push_back(c.to_json());
        }
        return crow::json::wvalue(list);
    });

    CROW_ROUTE(_app, "/category/<int>").methods(crow::HTTPMethod::DELETE)
    ([this](int id) {
        _service.deleteCategory(static_cast<uint64_t>(id));

        return crow::response(200, "Category deleted. Resources updated to default.");
    }); 
}
