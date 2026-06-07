#include "preset_service.h"
#include "../mappers/preset_mapper.h"
#include "../domain/domain.h"
#include <iostream>

PresetService::PresetService(std::shared_ptr<DbContext> context, ResourceService& resourceService)
    :  _context(context),
    _presetTable(_context->presets.open_table<dm::Preset>("presets")),
    _linkTable(_context->preset_resources.open_table<dm::PresetResource>("preset_resources")),
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

DTO::PresetDto PresetService::updatePreset(uint64_t id, const DTO::CreatePresetRequest& req)
{
    dm::Preset preset = PresetMapper::to_domain(req, req.user_id);
    bool status = _presetTable.update(id, preset);

    if (!status)
    {
        throw std::runtime_error("Preset not found or update failed");
    }

    return PresetMapper::to_dto(preset, id);
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
        // Перебираем с запасом, чтобы точно зацепить все индексы
        uint64_t total = _linkTable.quantity;
        for (uint64_t i = 1; i <= total + 5; ++i) {
            try {
                dm::PresetResource link = _linkTable.get(i);
                // Проверяем, что запись не пустая и совпадает по ID
                if (link.preset_id == presetId && link.resource_id == resourceId) {
                    _linkTable.remove(i);
                    std::cout << "[DB] Resource " << resourceId << " успешно отвязан от Preset " << presetId << " (Индекс: " << i << ")" << std::endl;
                    return; // Сразу выходим, дело сделано
                }
            } catch (...) { continue; }
        }
    } catch (const std::exception& e) {
        std::cerr << "Remove Resource from Preset Error: " << e.what() << std::endl;
    }
}

std::vector<uint64_t> PresetService::getResourceIdsForPreset(uint64_t presetId) {
    std::vector<uint64_t> resourceIds;
    
    // Перебираем всю таблицу связей
    for (uint64_t i = 1; i <= _linkTable.quantity + 1; ++i) {
        try {
            dm::PresetResource link = _linkTable.get(i);
            
            // ЖЕСТКАЯ ПРОВЕРКА: валидными считаются только те записи, 
            // где ID пресета совпадает, а ID ресурса не равен 0 и не равен мусорным значениям
            if (link.preset_id == presetId && link.resource_id != 0 && link.preset_id != 0 && link.resource_id < 999999) {
                resourceIds.push_back(link.resource_id);
            }
        } catch (...) { 
            continue; 
        }
    }
    return resourceIds;
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
