#pragma once

#include <cstdint>

namespace domain_models
{
    struct Preset
    {
        uint64_t id;
        char name[255];
        char description[1024];
        uint64_t user_id;
    };

    static_assert(sizeof(Preset) == 1296, "Preset size must be 1296 bytes");
}
