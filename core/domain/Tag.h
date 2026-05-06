#pragma once

#include <cstdint>

namespace dm
{
    struct Tag
    {
        char name[255];
        char description[1024];
        uint64_t user_id;
    };

    static_assert(sizeof(Tag) == 1288, "Tag size must be 1288 bytes");
}
