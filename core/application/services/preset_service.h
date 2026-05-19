#pragma once
#include "../../domain/Preset.h"
#include "../../domain/PresetResource.h"
#include "../dto/Preset.h"
#include "resource_service.h"
#include <vector>

class PresetService {
public:
    PresetService(Engine::Database& presetDb, Engine::Database& linkDb, ResourceService& resourceService);

    int createPreset(const DTO::CreatePresetRequest& req);
    DTO::PresetDto updatePreset(uint64_t id, const DTO::CreatePresetRequest& req);
    std::vector<DTO::PresetDto> getPresetsByUserId(uint64_t userId);
    void deletePreset(uint64_t id);

    void addResourceToPreset(uint64_t presetId, uint64_t resourceId);
    void removeResourceFromPreset(uint64_t presetId, uint64_t resourceId);
    void runPreset(uint64_t presetId);

private:
    Engine::Table<dm::Preset> _presetTable;
    Engine::Table<dm::PresetResource> _linkTable;
    ResourceService& _resourceService;
};
