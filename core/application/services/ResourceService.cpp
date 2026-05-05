#include <cstring>
#include <vector>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

#include "../domain/Resource.h"
#include "ResourceService.h" 
#include "../../infrastructure/install/include/hellnah/Engine/Database.h"

    ResourceService::ResourceService(Engine::Database& db) 
        : _table(db.open_table<dm::Resource>("resources")) {}

    bool ResourceService::createResource(dm::Resource& res) {
        try {
            res.id = _table.insert(res);
            return res.id >= 0;
        } catch (const std::exception& e) {
            std::cerr << "DB Insert Error: " << e.what() << std::endl;
            return false;
        }
    }

    std::vector<dm::Resource> ResourceService::getAllResources() {
        std::vector<dm::Resource> all;
        
        // DEBUG LOGS
        std::cout << "--- Scan Start ---" << std::endl;
        std::cout << "Table Quantity: " << _table.quantity << std::endl;

        // Спробуємо "пробити" базу наскрізь, ігноруючи quantity
        // Перебираємо перші 10 слотів незалежно від того, що каже лічильник
        for (uint64_t i = 0; i < 10; ++i) {
            try {
                dm::Resource res = _table.get(i);
                if (res.id != (uint64_t)-1 && res.id != 0) {
                    std::cout << "Found Resource! ID: " << res.id << " Name: " << res.name << std::endl;
                    all.push_back(res);
                }
            } catch (...) {
                // Просто ідемо далі
            }
        }
        
        std::cout << "Total found: " << all.size() << std::endl;
        std::cout << "--- Scan End ---" << std::endl;
        
        return all;
    }

    dm::Resource ResourceService::getResourceById(uint64_t id) {
        try {
            return _table.get(id);
        } catch (...) {
            dm::Resource empty{};
            empty.id = -1;
            return empty;
        }
    }

    void ResourceService::deleteResource(uint64_t id) {
        _table.remove(id);
    }

    bool ResourceService::launchResource(uint64_t id) {
        dm::Resource res = getResourceById(id);
        if (res.id == (uint64_t)-1) return false;

        std::cout << "[Linux Launch] Resource: " << res.name << " Path: " << res.path << std::endl;

        std::string cmd = "xdg-open \"" + std::string(res.path) + "\" &"; 

        int result = std::system(cmd.c_str());
        
        return result == 0;
    }
