#pragma once

#include <cstdint>

namespace dm
{
    struct Resource
    {
        char name[255];
        char description[1024];
        char path[512];
        uint64_t user_id;
        uint64_t category_id;
    };

    static_assert(sizeof(Resource) == 1808, "Resource size must be 1808 bytes");
}
