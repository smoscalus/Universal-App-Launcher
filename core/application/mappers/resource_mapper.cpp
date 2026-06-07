#include "resource_mapper.h"
#include <cstring>

DTO::ResourceDto ResourceMapper::to_dto(const dm::Resource& obj, uint64_t id) {
    return DTO::ResourceDto{
        id,
        std::string(obj.name),
        std::string(obj.description),
        std::string(obj.path),
        std::string(obj.avatar_url),
        obj.user_id,
        obj.category_id
    };
}

dm::Resource ResourceMapper::to_domain(const DTO::CreateResourceRequest& req) {
    dm::Resource res{};
    std::strncpy(res.name, req.name.c_str(), sizeof(res.name) - 1);
    std::strncpy(res.description, req.description.c_str(), sizeof(res.description) - 1);
    std::strncpy(res.path, req.path.c_str(), sizeof(res.path) - 1);
    std::strncpy(res.avatar_url, req.avatar_url.c_str(), sizeof(res.avatar_url) - 1);

    res.user_id = req.user_id;
    res.category_id = req.category_id;
    return res;
}
