#include "category_mapper.h"
#include <cstring>

DTO::CategoryDto CategoryMapper::to_dto(const dm::Category& obj, uint64_t id) {
    return DTO::CategoryDto{
        id,
        std::string(obj.name),
        std::string(obj.description),
        obj.user_id
    };
}

dm::Category CategoryMapper::to_domain(const DTO::CreateCategoryRequest& req, uint64_t real_user_id) {
    dm::Category cat{};
    
    std::strncpy(cat.name, req.name.c_str(), sizeof(cat.name) - 1);
    cat.name[sizeof(cat.name) - 1] = '\0';
    
    std::strncpy(cat.description, req.description.c_str(), sizeof(cat.description) - 1);
    cat.description[sizeof(cat.description) - 1] = '\0';
    
    cat.user_id = real_user_id; 
    
    return cat;
}
