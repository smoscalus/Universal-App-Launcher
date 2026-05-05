#pragma once

#include <cstdint>

namespace dm
{
    struct User
    {
        char name[255];
        char avatar_url[512];
        uint64_t created_at;
    };

    static_assert(sizeof(User) == 776, "User size must be 776 bytes");
}
