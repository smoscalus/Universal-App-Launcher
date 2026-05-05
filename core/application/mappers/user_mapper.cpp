#include "user_mapper.h"
#include <cstring>
#include <ctime>

DTO::UserDto UserMapper::to_dto(const dm::User& obj, uint64_t id) {
    return DTO::UserDto{
        id,
        std::string(obj.name),
        std::string(obj.avatar_url),
        obj.created_at
    };
}

dm::User UserMapper::to_domain(const DTO::CreateUserRequest& req) {
    dm::User user{};
    std::strncpy(user.name, req.name.c_str(), sizeof(user.name) - 1);
    std::strncpy(user.avatar_url, req.avatar_url.c_str(), sizeof(user.avatar_url) - 1);
    user.created_at = static_cast<uint64_t>(std::time(nullptr));
    return user;
}
