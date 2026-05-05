#pragma once

#include <cstdint>

namespace dm
{
    struct Category
    {
        char name[255];
        char description[1024];
        uint64_t user_id;
    };

    static_assert(sizeof(Category) == 1288, "Category size must be 1288 bytes");
}
