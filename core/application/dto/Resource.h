#pragma once
#include <string>

#include "../infrastructure/install/include/Crow/crow_all.h"

namespace DTO {
    struct ResourceDto {
        uint64_t id;
        std::string name;
        std::string description;
        std::string path;
        std::string avatar_url;
        uint64_t user_id;
        uint64_t category_id;

        crow::json::wvalue to_json() const {
            crow::json::wvalue j;
            j["id"] = id;
            j["name"] = name;
            j["description"] = description;
            j["path"] = path;
            j["avatar_url"] = avatar_url;
            j["user_id"] = user_id;
            j["category_id"] = category_id;
            return j;
        }
    };

    struct CreateResourceRequest {
        std::string name;
        std::string description;
        std::string path;
        std::string avatar_url;
        uint64_t user_id;
        uint64_t category_id;
    };

    struct CreateResourceResponse {
        uint64_t id;
        bool success;
        std::string message;

        crow::json::wvalue to_json() const {
            crow::json::wvalue j;
            j["id"] = id;
            j["success"] = success;
            j["message"] = message;
            return j;
        }
    };
}
