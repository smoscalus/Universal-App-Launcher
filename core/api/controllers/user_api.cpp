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

    CROW_ROUTE(_app, "/users")
    ([this]() {
        auto users = _service.getAllUsers();
        
        crow::json::wvalue res = crow::json::wvalue::list();
        int index = 0;
        for (const auto& user : users) {
            crow::json::wvalue u;
            u["id"] = user.id;
            u["name"] = user.name;
            res[index++] = std::move(u);
        }
        return crow::response(201, res);
    });

    CROW_ROUTE(_app, "/user").methods(crow::HTTPMethod::POST)
    ([this](const crow::request& req) {
        auto data = crow::json::load(req.body);
        if (!data) return crow::response(400, "Invalid JSON");

        if (!data.has("name") || std::string(data["name"].s()).empty()) {
            return crow::response(400, "Name is required");
        }

        DTO::CreateUserRequest createReq;
        createReq.name = data["name"].s();
        createReq.avatar_url = data.has("avatar_url") ? std::string(data["avatar_url"].s()) : "";

        int userId = _service.createUser(createReq);
        
        if (userId > 0) {
            crow::json::wvalue res;
            res["id"] = userId;
            res["name"] = createReq.name;
            return crow::response(201, res);
        }
        return crow::response(500, "Error authenticating user");
    });

    CROW_ROUTE(_app, "/user/<int>").methods(crow::HTTPMethod::DELETE)
    ([this](int id) {
        try {
            _service.deleteUser(static_cast<uint64_t>(id));
            return crow::response(200, "User and all data deleted cascade");
        } catch (const std::exception& e) {
            return crow::response(500, "Error deleting user");
        }
    });
}
