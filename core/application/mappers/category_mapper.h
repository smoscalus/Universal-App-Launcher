#pragma once
#include "../dto/Category.h"
#include "../../domain/Category.h"

class CategoryMapper {
public:
    static DTO::CategoryDto to_dto(const dm::Category& obj, uint64_t id);
    static dm::Category to_domain(const DTO::CreateCategoryRequest& req, uint64_t real_user_id);
};
