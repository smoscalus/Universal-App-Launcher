#pragma once
#include "../dto/User.h"
#include "../../domain/User.h"

class UserMapper {
public:
    static DTO::UserDto to_dto(const dm::User& obj, uint64_t id);
    static dm::User to_domain(const DTO::CreateUserRequest& req);
};
