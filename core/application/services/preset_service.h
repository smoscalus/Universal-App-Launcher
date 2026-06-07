#pragma once

#include "../../domain/Preset.h"
#include "../../domain/PresetResource.h"
#include "../dto/Preset.h"
#include "../../infrastructure/persistence/db_context.h"
#include "resource_service.h"
#include <vector>
#include <memory>

class PresetService {
public:
    PresetService(std::shared_ptr<DbContext> context, ResourceService& resourceService);

    int createPreset(const DTO::CreatePresetRequest& req);
    DTO::PresetDto updatePreset(uint64_t id, const DTO::CreatePresetRequest& req);
    std::vector<DTO::PresetDto> getPresetsByUserId(uint64_t userId);
    std::vector<uint64_t> getResourceIdsForPreset(uint64_t presetId);
    void deletePreset(uint64_t id);

    void addResourceToPreset(uint64_t presetId, uint64_t resourceId);
    void removeResourceFromPreset(uint64_t presetId, uint64_t resourceId);
    void runPreset(uint64_t presetId);

private:
    std::shared_ptr<DbContext> _context;
    Engine::Table<dm::Preset> _presetTable;
    Engine::Table<dm::PresetResource> _linkTable;
    ResourceService& _resourceService;
};
