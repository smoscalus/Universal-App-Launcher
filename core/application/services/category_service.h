#pragma once
#include "../../domain/Category.h"
#include "../../domain/Resource.h"
#include "../dto/Category.h"
#include "user_service.h"
#include <vector>

class CategoryService {
public:
    CategoryService(Engine::Database& db, UserService& userService);

    int createCategory(const DTO::CreateCategoryRequest& req);
    std::vector<DTO::CategoryDto> getCategoriesByUserId(uint64_t userId);
    void deleteCategory(uint64_t id);

private:
    Engine::Table<dm::Category> _table;
    Engine::Table<dm::Resource> _resourceTable; 
    UserService& _userService;
};
