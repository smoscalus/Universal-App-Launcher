#pragma once

#include "../../infrastructure/install/include/hellnah/Engine/Database.h"
#include "../../infrastructure/persistence/db_context.h"
#include "../dto/Resource.h"
#include "../../domain/Resource.h"
#include <vector>
#include <memory>

class ResourceService {
public:
    ResourceService(std::shared_ptr<DbContext> context);

    std::vector<DTO::ResourceDto> getResourcesByUserId(uint64_t userId);
    DTO::ResourceDto getResourceById(uint64_t id);
    int createResource(const DTO::CreateResourceRequest& req);
    DTO::ResourceDto updateResource(uint64_t id, const DTO::CreateResourceRequest& req);
    bool launchResource(uint64_t id);
    void deleteResource(uint64_t id);

private:
    std::shared_ptr<DbContext> _context;
    Engine::Table<dm::Resource> _table;
};