#include "user_service.h"
#include "../mappers/user_mapper.h"

UserService::UserService(Engine::Database& db) 
    : _table(db.open_table<dm::User>("users")) {}

DTO::UserDto UserService::getUserById(uint64_t id) {
    try {
        dm::User user = _table.get(id);
        if (user.name[0] == '\0') throw std::runtime_error("Empty slot");
        
        return UserMapper::to_dto(user, id);
    } catch (...) {
        return DTO::UserDto{0,"Guest", "", 0};
    }
}

int UserService::createUser(const DTO::CreateUserRequest& req) {
    try {
        dm::User user = UserMapper::to_domain(req);
        uint64_t newId = _table.insert(user);
        return static_cast<int>(newId);
    } catch (const std::exception& e) {
        std::cerr << "User Creation Error: " << e.what() << std::endl;
        return -1;
    }
}
