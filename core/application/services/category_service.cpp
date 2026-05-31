#include "category_service.h"
#include "../mappers/category_mapper.h"
#include <iostream>

    CategoryService::CategoryService(std::shared_ptr<DbContext> context, UserService& userService)
        : _context(context),
        _table(context->categories.open_table<dm::Category>("categories")),
        _resourceTable(context->resources.open_table<dm::Resource>("resources")),
        _userService(userService) {}

    int CategoryService::createCategory(const DTO::CreateCategoryRequest& req) {
        try {
            dm::Category cat = CategoryMapper::to_domain(req, req.user_id);
            
            uint64_t newId = _table.insert(cat);
            return static_cast<int>(newId);
        } catch (const std::exception& e) {
            std::cerr << "Category DB Insert Error: " << e.what() << std::endl;
            return -1;
        }
    }

    DTO::CategoryDto CategoryService::updateCategory(uint64_t id, const DTO::CreateCategoryRequest& req)
    {
        dm::Category cat = CategoryMapper::to_domain(req, req.user_id);
        bool status = _table.update(id, cat);

        if (!status)
        {
            throw std::runtime_error("Category not found or update failed");
        }

        return CategoryMapper::to_dto(cat, id);
    }

    std::vector<DTO::CategoryDto> CategoryService::getCategoriesByUserId(uint64_t userId) {
        std::vector<DTO::CategoryDto> userCategories;

        for (uint64_t i = 1; i <= _table.quantity + 1; ++i) {
            try {
                dm::Category cat = _table.get(i);

                if (cat.name[0] != '\0' && cat.user_id == userId) {
                    userCategories.push_back(CategoryMapper::to_dto(cat, i));
                }
            } catch (...) {
                continue;
            }
        }
        return userCategories;
    }

    void CategoryService::deleteCategory(uint64_t id) {
        try {
            _context->delete_category_cascade(id);
            std::cout << "[DB] Category " << id << " and all its resources removed." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Category Delete Error: " << e.what() << std::endl;
        }
    }
