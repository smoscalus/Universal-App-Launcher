#include "resource_service.h" 
#include "../mappers/resource_mapper.h" 
#include <cstring>
#include <vector>
#include <iostream>

    ResourceService::ResourceService(Engine::Database& db) 
        : _table(db.open_table<dm::Resource>("resources")) {}

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

    std::vector<DTO::ResourceDto> ResourceService::getAllResources() {
        std::vector<DTO::ResourceDto> allDto;

        uint64_t limit = _table.quantity + 2; 
        for (uint64_t i = 0; i <= limit; ++i) {
            try {
                dm::Resource res = _table.get(i);

                if (res.name[0] != '\0') {
                    allDto.push_back(ResourceMapper::to_dto(res, i));
                }
            } catch (...) {
                continue;
            }
        }
        return allDto;
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
