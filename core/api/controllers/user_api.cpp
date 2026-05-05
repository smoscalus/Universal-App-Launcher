#include "user_api.h"
#include "../../application/dto/User.h"
#include "../infrastructure/install/include/Crow/crow_all.h"

void UserController::setup_routes() {

    CROW_ROUTE(_app, "/user/<int>")
    ([this](int id) {
        auto userDto = _service.getUserById(static_cast<uint64_t>(id));
        
        if (userDto.name == "Guest" && userDto.created_at == 0) {
            return crow::response(404, "User not found");
        }
        
        return crow::response(userDto.to_json());
    });

    CROW_ROUTE(_app, "/user").methods(crow::HTTPMethod::POST)
    ([this](const crow::request& req) {
        auto data = crow::json::load(req.body);
        if (!data) return crow::response(400, "Invalid JSON");

        DTO::CreateUserRequest createReq;
        createReq.name = data["name"].s();
        createReq.avatar_url = data.has("avatar_url") ? std::string(data["avatar_url"].s()) : "";

        int newId = _service.createUser(createReq);
        
        if (newId > 0) {
            crow::json::wvalue res;
            res["id"] = newId;
            return crow::response(201, res);
        }
        return crow::response(500, "Error creating user");
    });
}
