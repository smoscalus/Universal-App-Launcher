#pragma once
#include "../dto/Tag.h"
#include "../../domain/Tag.h"

class TagMapper {
public:
    static DTO::TagDto to_dto(const dm::Tag& obj, uint64_t id);
    static dm::Tag to_domain(const DTO::CreateTagRequest& req, uint64_t real_user_id);
};
