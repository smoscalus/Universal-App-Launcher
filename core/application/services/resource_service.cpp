#include "resource_service.h" 
#include "../mappers/resource_mapper.h" 
#include <cstring>
#include <vector>
#include <iostream>

    ResourceService::ResourceService(std::shared_ptr<DbContext> context) 
        : _context(context),
        _table(_context->resources.open_table<dm::Resource>("resources")) {}

    int ResourceService::createResource(const DTO::CreateResourceRequest& req) {
        try {
            dm::Resource res = ResourceMapper::to_domain(req);
            
            uint64_t newId = _table.insert(res);
            return static_cast<int>(newId);
        } catch (const std::exception& e) {
            std::cerr << "DB Insert Error: " << e.what() << std::endl;
            return -1;
        }
    }

    DTO::ResourceDto ResourceService::getResourceById(uint64_t id) {
    try {
        dm::Resource res = _table.get(id);
        
        if (res.name[0] == '\0') {
            throw std::runtime_error("Resource not found");
        }

        return ResourceMapper::to_dto(res, id);
    } catch (...) {
        return DTO::ResourceDto{0, "", "", "", 0, 0};
    }
    }

    std::vector<DTO::ResourceDto> ResourceService::getResourcesByUserId(uint64_t userId) {
        std::vector<DTO::ResourceDto> userResources;

        for (uint64_t i = 1; i <= _table.quantity + 1; ++i) {
            try {
                dm::Resource res = _table.get(i);

                if (res.name[0] != '\0' && res.user_id == userId) {
                    userResources.push_back(ResourceMapper::to_dto(res, i));
                }
            } catch (...) {
                continue;
            }
        }
        
        return userResources;
    }

    DTO::ResourceDto ResourceService::updateResource(uint64_t id, const DTO::CreateResourceRequest& req)
    {
        dm::Resource res = ResourceMapper::to_domain(req);
        bool status = _table.update(id, res);

        if (!status)
        {
            throw std::runtime_error("Resource not found or update failed");
        }

        return ResourceMapper::to_dto(res, id);
    }


    void ResourceService::deleteResource(uint64_t id) {
        _table.remove(id);
    }
    
bool ResourceService::launchResource(uint64_t id) {
    try {
        dm::Resource res = _table.get(id);
        if (res.name[0] == '\0') return false;

        std::cout << "[Linux Launch] Resource: " << res.name << " Path: " << res.path << std::endl;

        std::string cmd = "xdg-open \"" + std::string(res.path) + "\" &"; 

        return std::system(cmd.c_str()) == 0;
    } catch (...) {
        return false;
    }
}