#include "tag_mapper.h"
#include <cstring>

DTO::TagDto TagMapper::to_dto(const dm::Tag& obj, uint64_t id) {
    return DTO::TagDto{
        id,
        std::string(obj.name),
        std::string(obj.description),
        obj.user_id
    };
}

dm::Tag TagMapper::to_domain(const DTO::CreateTagRequest& req, uint64_t real_user_id) {
    dm::Tag tag{};
    
    std::strncpy(tag.name, req.name.c_str(), sizeof(tag.name) - 1);
    tag.name[sizeof(tag.name) - 1] = '\0';
    
    std::strncpy(tag.description, req.description.c_str(), sizeof(tag.description) - 1);
    tag.description[sizeof(tag.description) - 1] = '\0';
    
    tag.user_id = real_user_id; 
    
    return tag;
}
