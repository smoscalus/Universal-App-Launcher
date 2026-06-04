#pragma once

#include <memory>

#include "../../infrastructure/install/include/hellnah/Engine/Database.h"
#include "../../infrastructure/persistence/db_context.h"
#include "../dto/User.h"
#include "../../domain/User.h"


class UserService {

public:
    UserService(std::shared_ptr<DbContext> context);

    DTO::UserDto getUserById(uint64_t id);
    std::vector<DTO::UserDto> getAllUsers();
    int createUser(const DTO::CreateUserRequest& req);
    bool existsByUsername(const std::string& name);
    void deleteUser(uint64_t id);
private:
    std::shared_ptr<DbContext> _context;
    Engine::Table<dm::User> _table;
};
