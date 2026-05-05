#pragma once
#include "../dto/Resource.h"
#include "../../domain/Resource.h"

class ResourceMapper {
public:
    static DTO::ResourceDto to_dto(const dm::Resource& obj, uint64_t id);
    static dm::Resource to_domain(const DTO::CreateResourceRequest& req);
};
