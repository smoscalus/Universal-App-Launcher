#pragma once
#include "../../infrastructure/install/include/hellnah/Engine/Database.h"
#include "../dto/User.h"
#include "../../domain/User.h"

class UserService {
private:
    Engine::Table<dm::User> _table;

public:
    UserService(Engine::Database& db);

    DTO::UserDto getUserById(uint64_t id);
    int createUser(const DTO::CreateUserRequest& req);
    void deleteUser(uint64_t id);
};
