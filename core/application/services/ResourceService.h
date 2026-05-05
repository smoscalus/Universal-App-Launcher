#pragma once

#include <vector>

#include "../../infrastructure/install/include/hellnah/Engine/Database.h"
#include "../domain/Resource.h"

class ResourceService {
private:
    Engine::Table<dm::Resource> _table;

public:
    ResourceService(Engine::Database& db);

    bool createResource(dm::Resource& res);
    std::vector<dm::Resource> getAllResources();
    dm::Resource getResourceById(uint64_t id);
    
    void deleteResource(uint64_t id);

    bool launchResource(uint64_t id);
};