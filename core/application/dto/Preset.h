#pragma once
#include <string>
#include <vector>
#include "../infrastructure/install/include/Crow/crow_all.h"

namespace DTO {
    struct PresetDto {
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

    struct CreatePresetRequest {
        std::string name;
        std::string description;
        uint64_t user_id;
    };
}
