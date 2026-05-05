#pragma once
#include <string>
#include "../infrastructure/install/include/Crow/crow_all.h"

namespace DTO {
    struct UserDto {
        uint64_t id;
        std::string name;
        std::string avatar_url;
        uint64_t created_at;

        crow::json::wvalue to_json() const {
            crow::json::wvalue j;
            j["name"] = name;
            j["avatar_url"] = avatar_url;
            j["created_at"] = created_at;
            return j;
        }
    };

    struct CreateUserRequest {
        std::string name;
        std::string avatar_url;
    };
}
