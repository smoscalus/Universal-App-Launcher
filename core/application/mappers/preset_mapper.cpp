#include "preset_mapper.h"
#include <cstring>

DTO::PresetDto PresetMapper::to_dto(const dm::Preset& obj, uint64_t id) {
    return DTO::PresetDto{
        id,
        std::string(obj.name),
        std::string(obj.description),
        obj.user_id
    };
}

dm::Preset PresetMapper::to_domain(const DTO::CreatePresetRequest& req, uint64_t real_user_id) {
    dm::Preset p{};
    std::strncpy(p.name, req.name.c_str(), sizeof(p.name) - 1);
    p.name[sizeof(p.name) - 1] = '\0';
    
    std::strncpy(p.description, req.description.c_str(), sizeof(p.description) - 1);
    p.description[sizeof(p.description) - 1] = '\0';
    
    p.user_id = real_user_id; 
    return p;
}
