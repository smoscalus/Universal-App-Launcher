#pragma once
#include "../dto/Preset.h"
#include "../../domain/Preset.h"

class PresetMapper {
public:
    static DTO::PresetDto to_dto(const dm::Preset& obj, uint64_t id);
    static dm::Preset to_domain(const DTO::CreatePresetRequest& req, uint64_t real_user_id);
};
