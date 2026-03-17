#pragma once

#include <cstdint>

struct User
{
    uint64_t id;
    char name[255];
    char avatar_url[512];
    uint64_t created_at;
};

static_assert(sizeof(User) == 784, "User size must be 784 bytes");
