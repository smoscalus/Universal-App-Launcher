#pragma once

#include <vector>
#include "../../infrastructure/install/include/hellnah/Engine/Database.h"
#include "../dto/Resource.h"
#include "../../domain/Resource.h"

class ResourceService {
private:
    Engine::Table<dm::Resource> _table;

public:
    ResourceService(Engine::Database& db);

    std::vector<DTO::ResourceDto> getResourcesByUserId(uint64_t userId);
    DTO::ResourceDto getResourceById(uint64_t id);
    int createResource(const DTO::CreateResourceRequest& req);
    bool launchResource(uint64_t id);
    void deleteResource(uint64_t id);
};