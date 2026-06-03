#pragma once
#include <filesystem>
#include <string>

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
};
