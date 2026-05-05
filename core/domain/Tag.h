#pragma once

#include <cstdint>

namespace dm
{
    struct Tag
    {
        char name[255];
        char description[1024];
    };

    static_assert(sizeof(Tag) == 1280, "Tag size must be 1280 bytes");
}
