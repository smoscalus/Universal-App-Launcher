#include "user_service.h"
#include "../mappers/user_mapper.h"

UserService::UserService(std::shared_ptr<DbContext> context) 
    :  _context(context),
    _table(_context->users.open_table<dm::User>("users")) {}

DTO::UserDto UserService::getUserById(uint64_t id) {
    try {
        dm::User user = _table.get(id);
        if (user.name[0] == '\0') throw std::runtime_error("Empty slot");
        
        return UserMapper::to_dto(user, id);
    } catch (...) {
        return DTO::UserDto{0,"Guest", "", 0};
    }
}

bool UserService::existsByUsername(const std::string& name) {
    for (uint64_t i = 1; i <= _table.quantity + 1; ++i) {
        try {
            dm::User user = _table.get(i);

            if (user.name[0] == '\0') continue;

            if (std::string(user.name) == name) {
                return true; 
            }

        } catch (...) {
            continue;
        }
    }
    return false;
}

int UserService::createUser(const DTO::CreateUserRequest& req) {
    try {
        for (uint64_t i = 1; i <= _table.quantity + 1; ++i) {
            try {
                dm::User user = _table.get(i);
                if (user.name[0] == '\0') continue;

                if (std::string(user.name) == req.name) {
                    return static_cast<int>(i); 
                }
            } catch (...) {
                continue;
            }
        }

        dm::User user = UserMapper::to_domain(req);
        uint64_t newId = _table.insert(user);
        return static_cast<int>(newId);

    } catch (const std::exception& e) {
        std::cerr << "User Authentication Error: " << e.what() << std::endl;
        return -1;
    }
}
