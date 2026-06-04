#pragma once
#include <filesystem>
#include <string>
#include <vector>

#include "../../domain/domain.h"
#include "install/include/hellnah/Engine/Database.h"

class DbContext {
public:
    Engine::Database resources;
    Engine::Database categories;
    Engine::Database users;
    Engine::Database tags;
    Engine::Database resource_tags;
    Engine::Database presets;
    Engine::Database preset_resources;

    DbContext(const std::string& db_dir = "hellnah") 
        : resources((db_dir + "/resources.hellnot").c_str())
        , categories((db_dir + "/categorys.hellnot").c_str())
        , users((db_dir + "/users.hellnot").c_str())
        , tags((db_dir + "/tags.hellnot").c_str())
        , resource_tags((db_dir + "/resource_tags.hellnot").c_str())
        , presets((db_dir + "/presets.hellnot").c_str())
        , preset_resources((db_dir + "/preset_resources.hellnot").c_str()) {}

    void delete_category_cascade(uint64_t id) {
        auto resourceTable = resources.open_table<dm::Resource>("resources");
        auto categoryTable = categories.open_table<dm::Category>("categories");

        for (uint64_t i = 0; i < resourceTable.quantity; ++i) {
            try {
                dm::Resource res = resourceTable.get(i);
                if (res.name[0] != '\0' && res.category_id == id) {
                    resourceTable.remove(i);
                }
            } catch (...) { continue; }
        }
        categoryTable.remove(id);
    }

    void delete_user_cascade(uint64_t userId) {
        auto resourceTable = resources.open_table<dm::Resource>("resources");
        auto categoryTable = categories.open_table<dm::Category>("categories");
        auto userTable = users.open_table<dm::User>("users");

        std::vector<uint64_t> userCategoryIds;
        for (uint64_t i = 1; i <= categoryTable.quantity + 1; ++i) {
            try {
                dm::Category cat = categoryTable.get(i);
                if (cat.name[0] != '\0' && cat.user_id == userId) {
                    userCategoryIds.push_back(i);
                }
            } catch (...) { continue; }
        }

        for (uint64_t i = 1; i <= resourceTable.quantity + 1; ++i) {
            try {
                dm::Resource res = resourceTable.get(i);
                if (res.name[0] != '\0') {
                    for (uint64_t catId : userCategoryIds) {
                        if (res.category_id == catId) {
                            resourceTable.remove(i);
                            break;
                        }
                    }
                }
            } catch (...) { continue; }
        }
        for (uint64_t catId : userCategoryIds) {
            try {
                categoryTable.remove(catId);
            } catch (...) { continue; }
        }

        userTable.remove(userId);
    }
};
