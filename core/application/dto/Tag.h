#pragma once
#include <string>
#include <cstdint>
#include "../infrastructure/install/include/Crow/crow_all.h"

namespace DTO {
    struct TagDto {
        uint64_t id;
        std::string name;
        std::string description;
        uint64_t user_id;

        crow::json::wvalue to_json() const {
            crow::json::wvalue j;
            j["id"] = id;
            j["name"] = name;
            j["description"] = description;
            j["user_id"] = user_id;
            return j;
        }
    };

    struct CreateTagRequest {
        std::string name;
        std::string description;
        uint64_t user_id;
    };

    struct CreateTagResponse {
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