#pragma once

#include "../../domain/Category.h"
#include "../../domain/Resource.h"
#include "../dto/Category.h"
#include "user_service.h"
#include "../../infrastructure/persistence/db_context.h"
#include <vector>
#include <memory>

class CategoryService {
public:
    CategoryService(std::shared_ptr<DbContext> context, UserService& userService);

    int createCategory(const DTO::CreateCategoryRequest& req);
    DTO::CategoryDto updateCategory(uint64_t id, const DTO::CreateCategoryRequest& req);
    std::vector<DTO::CategoryDto> getCategoriesByUserId(uint64_t userId);
    void deleteCategory(uint64_t id);

private:
    std::shared_ptr<DbContext> _context;
    Engine::Table<dm::Category> _table;
    Engine::Table<dm::Resource> _resourceTable; 
    UserService& _userService;
};
