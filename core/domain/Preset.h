#pragma once

#include <cstdint>

namespace dm
{
    struct Preset
    {
        char name[255];
        char description[1024];
        uint64_t user_id;
    };

    static_assert(sizeof(Preset) == 1288, "Preset size must be 1288 bytes");
}
