#include "preset_service.h"
#include "../mappers/preset_mapper.h"
#include <iostream>

PresetService::PresetService(Engine::Database& presetDb, Engine::Database& linkDb, ResourceService& resourceService)
    : _presetTable(presetDb.open_table<dm::Preset>("presets")),
      _linkTable(linkDb.open_table<dm::PresetResource>("preset_resources")),
      _resourceService(resourceService) {}

int PresetService::createPreset(const DTO::CreatePresetRequest& req) {
    try {
        dm::Preset preset = PresetMapper::to_domain(req, req.user_id);
        uint64_t newId = _presetTable.insert(preset);
        return static_cast<int>(newId);
    } catch (const std::exception& e) {
        std::cerr << "Preset DB Insert Error: " << e.what() << std::endl;
        return -1;
    }
}

std::vector<DTO::PresetDto> PresetService::getPresetsByUserId(uint64_t userId) {
    std::vector<DTO::PresetDto> userPresets;
    for (uint64_t i = 1; i <= _presetTable.quantity + 1; ++i) {
        try {
            dm::Preset p = _presetTable.get(i);
            if (p.name[0] != '\0' && p.user_id == userId) {
                userPresets.push_back(PresetMapper::to_dto(p, i));
            }
        } catch (...) { continue; }
    }
    return userPresets;
}

void PresetService::deletePreset(uint64_t id) {
    try {
        _presetTable.remove(id);
        std::cout << "[DB] Preset " << id << " removed." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Preset Delete Error: " << e.what() << std::endl;
    }
}

void PresetService::addResourceToPreset(uint64_t presetId, uint64_t resourceId) {
    try {
        dm::PresetResource link;
        link.preset_id = presetId;
        link.resource_id = resourceId;
        _linkTable.insert(link);
        std::cout << "[DB] Resource " << resourceId << " added to Preset " << presetId << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Add Resource to Preset Error: " << e.what() << std::endl;
    }
}

void PresetService::removeResourceFromPreset(uint64_t presetId, uint64_t resourceId) {
    try {
        for (uint64_t i = 1; i <= _linkTable.quantity + 1; ++i) {
            try {
                dm::PresetResource link = _linkTable.get(i);
                if (link.preset_id == presetId && link.resource_id == resourceId) {
                    _linkTable.remove(i);
                    std::cout << "[DB] Resource " << resourceId << " removed from Preset " << presetId << std::endl;
                    break;
                }
            } catch (...) { continue; }
        }
    } catch (const std::exception& e) {
        std::cerr << "Remove Resource from Preset Error: " << e.what() << std::endl;
    }
}

void PresetService::runPreset(uint64_t presetId) {
    std::cout << "[ACTION] Running all resources in preset " << presetId << "..." << std::endl;
    
    for (uint64_t i = 1; i <= _linkTable.quantity + 1; ++i) {
        try {
            dm::PresetResource link = _linkTable.get(i);
            
            if (link.preset_id == presetId) {
                std::cout << "[ACTION] Launching resource ID: " << link.resource_id << std::endl;
                _resourceService.launchResource(link.resource_id);
            }
        } catch (...) {
            continue;
        }
    }
}
