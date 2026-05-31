#pragma once

#include "../../infrastructure/install/include/hellnah/Engine/Database.h"
#include "../../infrastructure/persistence/db_context.h"
#include "../dto/User.h"
#include "../../domain/User.h"
#include <memory>

class UserService {

public:
    UserService(std::shared_ptr<DbContext> context);

    DTO::UserDto getUserById(uint64_t id);
    int createUser(const DTO::CreateUserRequest& req);
    void deleteUser(uint64_t id);
private:
    std::shared_ptr<DbContext> _context;
    Engine::Table<dm::User> _table;
};
